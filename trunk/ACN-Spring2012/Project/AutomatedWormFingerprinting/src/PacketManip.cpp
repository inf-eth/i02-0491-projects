/*
# PacketManip.cpp
#
# Copyright (c) 2012 Attique Dawood, Awais Aslam
#
# This file is part of Automated Worm Fingerprinting.
#
# Automated Worm Fingerprinting is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Automated Worm Fingerprinting is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################
*/

#include <PacketManip.h>
#include <cstdlib>			// exit()
#include <ctime>
#include <iostream>
#include <bitset>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::bitset;

void packet_capture_callback(u_char *, const struct pcap_pkthdr*, const u_char*);

CPacketManip::CPacketManip (char *pdev, char *pfilter)
{
	// ask pcap for the network address and mask of the device
	if (pcap_lookupnet (pdev, &netp, &maskp, errbuf) == -1)
	{
		cerr << "ERROR pcap_lookupnet(): " << errbuf << endl;
		exit(-1);
	}

	// open device for reading this time lets set it in promiscuous mode so we can monitor traffic to another machine
	descr = pcap_open_live (pdev, BUFSIZ, 1, -1, errbuf);
	if(descr == NULL)
	{
		cerr << "ERROR pcap_open_live(): " << errbuf << endl;
		exit(-1);
	}

	// Lets try and compile the program.. non-optimized
	if (pcap_compile (descr, &fp, pfilter, 0, netp) == -1)
	{
		cerr << "ERROR calling pcap_compile()" << endl;
		exit(-1);
	}

	// set the compiled program as the filter
	if (pcap_setfilter (descr, &fp) == -1)
	{
		cerr << "ERROR setting filter" << endl;
		exit(1);
	}
	cout << "Device: " << pdev << endl;
	cout << "Filter: " << pfilter << endl;
}

void CPacketManip::Initialize (char *pdev, char *pfilter)
{
	#ifdef WIN32
	// If no interface specified ask for one.
	if (pdev == NULL)
	{
		pcap_if_t *alldevs;
		pcap_if_t *d;
		int inum;
		int i=0;
	
		/* Retrieve the device list on the local machine */
		if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
		{
			cerr << "Error in pcap_findalldevs: " << errbuf << endl;
			exit(-1);
		}
	
		/* Print the list */
		for(d=alldevs; d; d=d->next)
		{
			cout << ++i << "." << d->name << endl;
			if (d->description)
				cout << "Description: " << d->description << endl;
			else
				cerr<< "(No description available)" << endl;
		}
	
		if(i==0)
		{
			cerr << "No interfaces found! Make sure WinPcap is installed." << endl;
			return;
		}
	
		cout << "Enter the interface number (1-" << i << "):" << endl;
		cin >> inum;
	
		if(inum < 1 || inum > i)
		{
			cerr << "Interface number out of range." << endl;
			/* Free the device list */
			pcap_freealldevs(alldevs);
			return;
		}

		/* Jump to the selected adapter */
		for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
		pdev = new char[strlen(d->name)+1];
		strcpy (pdev, d->name);
		pcap_freealldevs(alldevs);
	}
	#else
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL)
	{
		cerr << "Couldn't find default device: " << errbuf << endl;
		return(-1);
	}
	#endif

	// ask pcap for the network address and mask of the device
	if (pcap_lookupnet (pdev, &netp, &maskp, errbuf) == -1)
	{
		cerr << "ERROR pcap_lookupnet(): " << errbuf << endl;
		exit(-1);
	}

	// open device for reading this time lets set it in promiscuous mode so we can monitor traffic to another machine
	descr = pcap_open_live (pdev, BUFSIZ, 1, -1, errbuf);
	if(descr == NULL)
	{
		cerr << "ERROR pcap_open_live(): " << errbuf << endl;
		exit(-1);
	}

	// Lets try and compile the program.. non-optimized
	if (pcap_compile (descr, &fp, pfilter, 0, netp) == -1)
	{
		cerr << "ERROR calling pcap_compile()" << endl;
		exit(-1);
	}

	// set the compiled program as the filter
	if (pcap_setfilter (descr, &fp) == -1)
	{
		cerr << "ERROR setting filter" << endl;
		exit(1);
	}
	cout << "Device: " << pdev << endl;
	cout << "Filter: " << pfilter << endl;
}
void CPacketManip::Loop ()
{
	pcap_loop (descr, -1, packet_capture_callback, NULL);
}

// All the work needs to be done here.
void packet_capture_callback(u_char *useless,const struct pcap_pkthdr* header,const u_char* packet)
{
	if (!header)
		return;
	
	// TODO: Comment this to stop some spam onscreen or set the appropriate filter program in main() if running on network.
	cout << " Recieved a packet at: " << ctime((const time_t*)&header->ts.tv_sec) << endl;

	// Header pointers.
	const struct sniff_ethernet *ethernet = NULL; /* The ethernet header */
	const struct sniff_ip *ip = NULL; /* The IP header */
	const struct sniff_tcp *tcp = NULL; /* The TCP header */
	const u_char *payload = NULL; /* Packet payload */

	// ************************** Pointer Initialization *********************************
	// packet* is the starting address of captured packet stored in memory.
	ethernet = (struct sniff_ethernet*)(packet);			// First thing in a packet is ethernet header.
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);		// IP header comes after ethernet header.
	// *************************************************************************************

	u_int size_ip;		// IP header length.
	size_ip = IP_HL(ip)*4;		// IP_HL is a macro for ((ip)->ip_vhl) & 0x0f;
	// IP_HL(ip) gives length in 32bit words. Multiplication by 4 gives length in bytes.
	if (size_ip < 20)
	{
		cerr << "ERROR: Invalid IP header length: " << size_ip << " bytes." << endl;
		return;
	}

	// TODO: Packet capturing condition. Only process packets with the specified protocol, and port numbers.
	// TODO: Process UDP packets and display their headers.
	// Default condition is to capture only packets with TCP protocol and source port = 6000 or 6001
	// ntohs(tcp->th_sport) gives source port
	// ntohs(tcp->th_dport) gives destination port
	// Protocols are IPPROTO_IP, IPPROTO_UDP, IPPROTO_ICMP etc.
	if ( ip && ip->ip_p == IPPROTO_TCP && (ntohs(tcp->th_sport) == 6000 || ntohs(tcp->th_sport) == 6001) )
	{
		// TODO: Process the headers. Display IP header.
		u_int size_tcp;		// TCP header legnth.
		cout << " Recieved a TCP packet at: " << ctime((const time_t*)&header->ts.tv_sec) << endl;

		tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);		// TCP header follows ethernet and IP headers.
		size_tcp = TH_OFF(tcp)*4;	// TH_OFF(tcp) is a macro for ((ip)->ip_vhl) >> 4;
		// TH_OFF(tcp) gives length of TCP header in 32bit words. Multiplication by 4 gives length in bytes.
		if (size_tcp < 20)
		{
			cerr << "ERROR: Invalid TCP header length: " << size_tcp << " bytes." << endl;
			return;
		}
		payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);	// Payload or data in packet.

		/* define/compute tcp payload (segment) offset */
		//payload = (packet + SIZE_ETHERNET + size_ip + size_tcp);
		/* compute tcp payload (segment) size */
		u_int size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
		u_int PacketSize = size_payload + size_ip + SIZE_ETHERNET + size_tcp;
		cout << "Payload size: " << size_payload << endl;
		cout << "Packet size: " << PacketSize << endl;

		// bitset is a C++ class for bit manipulation. It has overloaded [], << and >> operators for easy access to individual bits and input/output.
		// Using bitset of size 8 to display bits in a single character (byte).
		bitset<8> DataOffset;
		bitset<8> Flags;
		DataOffset = tcp->th_offx2;		// tcp->th_offx2 points to offset character.
		Flags = tcp->th_flags;			// tcp->th_flags character contain flag bits.

		cout << "******* TCP Header *******" << endl
			 << "Source Port:         " << ntohs(tcp->th_sport) << endl
			 << "Destination Port:    " << ntohs(tcp->th_dport) << endl
			 << "Sequence No.:        " << ntohl (tcp->th_seq) << endl
			 << "Acknowledgement No.: " << ntohl(tcp->th_ack) << endl
			 << "TCP header length:   " << size_tcp << endl
			 << "Data offset flags:   " << DataOffset << endl
			 << "Flags:               " << endl
										<< "\t\t" << "FIN:    " << Flags[0] << endl
										<< "\t\t" << "SYN:    " << Flags[1] << endl
										<< "\t\t" << "RESET:  " << Flags[2] << endl
										<< "\t\t" << "PUSH:   " << Flags[3] << endl
										<< "\t\t" << "ACK:    " << Flags[4] << endl
										<< "\t\t" << "URGENT: " << Flags[5] << endl
										<< "\t\t" << "ECE:    " << Flags[6] << endl
										<< "\t\t" << "CWR:    " << Flags[7] << endl
			 << "Window size:         " << ntohs(tcp->th_win) << endl
			 << "Checksum:            " << ntohs(tcp->th_sum) << endl
			 << "Urgent Pointer:      " << ntohs(tcp->th_urp) << endl;

		// TODO: Display payload data.
		if (size_payload != 0)
		{
			cout << "Payload: ";
			u_int Payload_Offset = SIZE_ETHERNET + size_ip + size_tcp;
			for (u_int i=0; i<size_payload; i++)
			{
				cout << packet[i + Payload_Offset];
			}
			cout << endl;
		}
		cout << "*************************" << endl;

	}
	else
		return;		// Not a packet of our interest.
}
