/*
# PacketManip.cpp
#
# Copyright (c) 2012 Attique Dawood, Awais Aslam
#
# This file is part of Automated Worm Fingerprinting. It contains code from sniffex.c.
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
/*
* sniffex.c
*
* Sniffer example of TCP/IP packet capture using libpcap.
*
* Version 0.1.1 (2005-07-05)
* Copyright (c) 2005 The Tcpdump Group
*
* This software is intended to be used as a practical example and
* demonstration of the libpcap library; available at:
* http://www.tcpdump.org/
*
****************************************************************************
*
* This software is a modification of Tim Carstens' "sniffer.c"
* demonstration source code, released as follows:
*
* sniffer.c
* Copyright (c) 2002 Tim Carstens
* 2002-01-07
* Demonstration of using libpcap
* timcarst -at- yahoo -dot- com
*
* "sniffer.c" is distributed under these terms:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 4. The name "Tim Carstens" may not be used to endorse or promote
*    products derived from this software without prior written permission
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* <end of "sniffer.c" terms>
*
* This software, "sniffex.c", is a derivative work of "sniffer.c" and is
* covered by the following terms:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Because this is a derivative work, you must comply with the "sniffer.c"
*    terms reproduced above.
* 2. Redistributions of source code must retain the Tcpdump Group copyright
*    notice at the top of this source file, this list of conditions and the
*    following disclaimer.
* 3. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 4. The names "tcpdump" or "libpcap" may not be used to endorse or promote
*    products derived from this software without prior written permission.
*
* THERE IS ABSOLUTELY NO WARRANTY FOR THIS PROGRAM.
* BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
* FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
* OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
* PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
* OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
* TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
* PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
* REPAIR OR CORRECTION.
*
* IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
* WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
* REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
* INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
* OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
* TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
* YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
* PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGES.
* <end of "sniffex.c" terms>
*
****************************************************************************
*
* Below is an excerpt from an email from Guy Harris on the tcpdump-workers
* mail list when someone asked, "How do I get the length of the TCP
* payload?" Guy Harris' slightly snipped response (edited by him to
* speak of the IPv4 header length and TCP data offset without referring
* to bitfield structure members) is reproduced below:
*
* The Ethernet size is always 14 bytes.
*
* <snip>...</snip>
*
* In fact, you *MUST* assume the Ethernet header is 14 bytes, *and*, if
* you're using structures, you must use structures where the members
* always have the same size on all platforms, because the sizes of the
* fields in Ethernet - and IP, and TCP, and... - headers are defined by
* the protocol specification, not by the way a particular platform's C
* compiler works.)
*
* The IP header size, in bytes, is the value of the IP header length,
* as extracted from the "ip_vhl" field of "struct sniff_ip" with
* the "IP_HL()" macro, times 4 ("times 4" because it's in units of
* 4-byte words).  If that value is less than 20 - i.e., if the value
* extracted with "IP_HL()" is less than 5 - you have a malformed
* IP datagram.
*
* The TCP header size, in bytes, is the value of the TCP data offset,
* as extracted from the "th_offx2" field of "struct sniff_tcp" with
* the "TH_OFF()" macro, times 4 (for the same reason - 4-byte words).
* If that value is less than 20 - i.e., if the value extracted with
* "TH_OFF()" is less than 5 - you have a malformed TCP segment.
*
* So, to find the IP header in an Ethernet packet, look 14 bytes after
* the beginning of the packet data.  To find the TCP header, look
* "IP_HL(ip)*4" bytes after the beginning of the IP header.  To find the
* TCP payload, look "TH_OFF(tcp)*4" bytes after the beginning of the TCP
* header.
*
* To find out how much payload there is:
*
* Take the IP *total* length field - "ip_len" in "struct sniff_ip"
* - and, first, check whether it's less than "IP_HL(ip)*4" (after
* you've checked whether "IP_HL(ip)" is >= 5).  If it is, you have
* a malformed IP datagram.
*
* Otherwise, subtract "IP_HL(ip)*4" from it; that gives you the length
* of the TCP segment, including the TCP header.  If that's less than
* "TH_OFF(tcp)*4" (after you've checked whether "TH_OFF(tcp)" is >= 5),
* you have a malformed TCP segment.
*
* Otherwise, subtract "TH_OFF(tcp)*4" from it; that gives you the
* length of the TCP payload.
*
* Note that you also need to make sure that you don't go past the end
* of the captured data in the packet - you might, for example, have a
* 15-byte Ethernet packet that claims to contain an IP datagram, but if
* it's 15 bytes, it has only one byte of Ethernet payload, which is too
* small for an IP header.  The length of the captured data is given in
* the "caplen" field in the "struct pcap_pkthdr"; it might be less than
* the length of the packet, if you're capturing with a snapshot length
* other than a value >= the maximum packet size.
* <end of response>
*
****************************************************************************
*
* Example compiler command-line for GCC:
*   gcc -Wall -o sniffex sniffex.c -lpcap
*
****************************************************************************
*
* Code Comments
*
* This section contains additional information and explanations regarding
* comments in the source code. It serves as documentaion and rationale
* for why the code is written as it is without hindering readability, as it
* might if it were placed along with the actual code inline. References in
* the code appear as footnote notation (e.g. [1]).
*
* 1. Ethernet headers are always exactly 14 bytes, so we define this
* explicitly with "#define". Since some compilers might pad structures to a
* multiple of 4 bytes - some versions of GCC for ARM may do this -
* "sizeof (struct sniff_ethernet)" isn't used.
*
* 2. Check the link-layer type of the device that's being opened to make
* sure it's Ethernet, since that's all we handle in this example. Other
* link-layer types may have different length headers (see [1]).
*
* 3. This is the filter expression that tells libpcap which packets we're
* interested in (i.e. which packets to capture). Since this source example
* focuses on IP and TCP, we use the expression "ip", so we know we'll only
* encounter IP packets. The capture filter syntax, along with some
* examples, is documented in the tcpdump man page under "expression."
* Below are a few simple examples:
*
* Expression			Description
* ----------			-----------
* ip					Capture all IP packets.
* tcp					Capture only TCP packets.
* tcp port 80			Capture only TCP packets with a port equal to 80.
* ip host 10.1.2.3		Capture all IP packets to or from host 10.1.2.3.
*
****************************************************************************
*
*/

#include <PacketManip.h>
#include <cstdlib>			// exit()
#include <ctime>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <cctype>
#include <arpa/inet.h>		// inet_ntoa()

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::bitset;
using std::hex;
using std::dec;
using std::setfill;
using std::setw;

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

void CPacketManip::Initialize (const char *pdev, const char *pfilter)
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
	
		char *tdev = new char[strlen(d->name)+1];
		strcpy (tdev, d->name);
		pdev = tdev;
		pcap_freealldevs(alldevs);
	}
	#else
	if (pdev == NULL)
	{
		pdev = pcap_lookupdev(errbuf);
		if (pdev == NULL)
		{
			cerr << "Couldn't find default device: " << errbuf << endl;
			return;
		}
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

// TODO: All the work needs to be done here.
void packet_capture_callback(u_char *useless,const struct pcap_pkthdr* header,const u_char* packet)
{
	// TODO: Comment this to stop some spam onscreen or set the appropriate filter program in main() if running on network.
	cout << "Recieved a packet at: " << ctime((const time_t*)&header->ts.tv_sec);

	// Header pointers.
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip; /* The IP header */
	const struct sniff_tcp *tcp; /* The TCP header */
	const u_char *payload; /* Packet payload */

	u_int size_ip;		// IP header length.
	u_int size_tcp;		// TCP header legnth.

	// ************************** Pointer Initialization *********************************
	// packet* is the starting address of captured packet stored in memory.
	ethernet = (struct sniff_ethernet*)(packet);			// First thing in a packet is ethernet header.
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);		// IP header comes after ethernet header.

	size_ip = IP_HL(ip)*4;		// IP_HL is a macro for ((ip)->ip_vhl) & 0x0f;
								// IP_HL(ip) gives length in 32bit words. Multiplication by 4 gives length in bytes.
	if (size_ip < 20)
	{
		cerr << "ERROR: Invalid IP header length: " << size_ip << " bytes." << endl;
		return;
	}

	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);		// TCP header follows ethernet and IP headers.
	size_tcp = TH_OFF(tcp)*4;	// TH_OFF(tcp) is a macro for ((ip)->ip_vhl) >> 4;
								// TH_OFF(tcp) gives length of TCP header in 32bit words. Multiplication by 4 gives length in bytes.
	if (size_tcp < 20)
	{
		cerr << "ERROR: Invalid TCP header length: " << size_tcp << " bytes." << endl;
		return;
	}

	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);	// Payload or data in packet.
	// *************************************************************************************


	// TODO: Packet capturing condition. Only process packets with the specified protocol, and port numbers.
	// TODO: Process UDP packets and display their headers.
	// Default condition is to capture only packets with TCP protocol and source port = 6000 or 6001
	// ntohs(tcp->th_sport) gives source port
	// ntohs(tcp->th_dport) gives destination port
	// Protocols are IPPROTO_IP, IPPROTO_UDP, IPPROTO_ICMP etc.
	if (ip)
	{
		// TODO: Process the headers. Display IP header.
		cout << "Recieved a TCP packet at: " << ctime((const time_t*)&header->ts.tv_sec) << endl;
		
		// print source and destination IP addresses
		cout << "Src IP: " << inet_ntoa(ip->ip_src) << endl;
		cout << "Dst IP: " << inet_ntoa(ip->ip_dst) << endl;

		// determine protocol
		switch(ip->ip_p)
		{
			case IPPROTO_TCP:
				cout << "Protocol: TCP" << endl;
				break;
			case IPPROTO_UDP:
				cout << "Protocol: UDP" << endl;
				return;
			case IPPROTO_ICMP:
				cout << "Protocol: ICMP" << endl;
				return;
			case IPPROTO_IP:
				cout << "Protocol: IP" << endl;
				return;
			default:
				cout << "Protocol: unknown" << endl;
				return;
		}

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
			cout << "Payload: " << endl;
			u_int Payload_Offset = SIZE_ETHERNET + size_ip + size_tcp;
			print_payload (packet+Payload_Offset, size_payload);
			/*
			for (int i=0; i<size_payload; i++)
			{
				cout << packet[i + Payload_Offset];
			}
			cout << endl;
			*/
		}
		cout << "*************************" << endl;

	}
	else
		return;		// Not a packet of our interest.
}

/*
	print data in rows of 16 bytes: offset   hex   ascii

	00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
*/
void print_hex_ascii_line(const u_char *payload, int len, int offset)
{
	int i;
	int gap;
	const u_char *ch;

	// offset
	cout << hex << setfill('0') << setw(4) << offset << dec << "   ";

	// hex
	ch = payload;
	for(i = 0; i < len; i++)
	{
		cout << hex << setfill('0') << setw(2) << (int)*ch << dec;//printf("%02x ", *ch);
		ch++;
		// print extra space after 8th byte for visual aid
		if (i == 7)
			cout << " ";
	}
	// print space to handle line less than 8 bytes
	if (len < 8)
		cout << " ";

	// fill hex gap with spaces if not full line
	if (len < 16)
	{
		gap = 16 - len;
		for (i = 0; i < gap; i++)
		{
			cout << "   ";
		}
	}
	cout << "   ";

	// ascii (if printable
	ch = payload;
	for(i = 0; i < len; i++)
	{
		if (isprint(*ch))
			cout << *ch;
		else
			cout << ".";
		ch++;
	}
	cout << endl;

	return;
}

//
// print packet payload data (avoid printing binary data)
//
void print_payload(const u_char *payload, int len)
{
	int len_rem = len;
	int line_width = 16;			// number of bytes per line
	int line_len;
	int offset = 0;					// zero-based offset counter
	const u_char *ch = payload;

	if (len <= 0)
		return;

	// data fits on one line
	if (len <= line_width)
	{
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	// data spans multiple lines
	for ( ;; )
	{
		// compute current line length
		line_len = line_width % len_rem;
		// print line
		print_hex_ascii_line(ch, line_len, offset);
		// compute total remaining
		len_rem = len_rem - line_len;
		// shift pointer to remaining bytes to print
		ch = ch + line_len;
		// add offset
		offset = offset + line_width;
		// check if we have line width chars or less
		if (len_rem <= line_width)
		{
			// print last line and get out
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}
	return;
}
