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
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
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
#include <Timing.h>
#include <cstdlib>			// exit()
#include <ctime>
#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <cctype>
#include <openssl/sha.h>	// SHA1()

#ifdef WIN32
#include <process.h>
#include <Windows.h>
#include <WinSock2.h>
#define CLOSE closesocket
#define THREAD_RETURN_TYPE void
#else
#include <arpa/inet.h>		// inet_ntoa()
#include <netdb.h>			// gethostbyname(), connect(), send(), recv()
#include <pthread.h>
#define CLOSE close
#define THREAD_RETURN_TYPE void*
#endif

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::fstream;
using std::bitset;
using std::hex;
using std::dec;
using std::setfill;
using std::setw;
using std::fill;

#ifdef WIN32
HANDLE tGarbageCollector;
HANDLE tReceiver;
HANDLE tLogger;
HANDLE MutexLock = CreateMutex (NULL, FALSE, NULL);
#else
pthread_t tGarbageCollector;
pthread_t tReceiver;
pthread_t tLogger;
pthread_mutex_t MutexLock = PTHREAD_MUTEX_INITIALIZER;
#endif

// Threads.
THREAD_RETURN_TYPE GarbageCollector (void *);
THREAD_RETURN_TYPE Receiver (void *);
THREAD_RETURN_TYPE Logger (void *);

// Network related.
// ****************************************** #Defintions ***********************************************
#define  MAXBUFFERSIZE		512		// Maximum default buffersize.
#define  SERVERPORT		   6011		// Server will be listening on this port by default.
// ******************************************************************************************************

// *********************************************** Globals ************************************************
int SocketFD;

struct hostent *he;
struct sockaddr_in ServerAddress;
struct sockaddr_in ClientAddress;

// Buffer.
char Buffer[MAXBUFFERSIZE];
int NumOfBytesSent;
int NumOfBytesReceived;

// Miscellaneous Variables.
int Yes = 1;
int AddressLength;
int errorcheck;
#ifdef WIN32
int sin_size;
#else
socklen_t sin_size;
#endif

struct SignatureData
{
	unsigned char Key[KEY_LENGTH];
	unsigned short th_sport;
	unsigned short th_dport;
	in_addr ip_src;
	in_addr ip_dst;
};
// ********************************************************************************************************

// Global Packet Manipulation object.
CPacketManip PacketCapture;

// Packet capture callback is called everytime a packet is received.
void packet_capture_callback(u_char *, const struct pcap_pkthdr*, const u_char*);

// ProcessPacket() is called everytime a packet is received with non-zero payload.
void ProcessPacket (unsigned char *, unsigned short, unsigned short, in_addr, in_addr);

// Default constructor.
CPacketManip::CPacketManip ():
								dev(NULL),
								descr(NULL),
								ContentPrevalenceThreshold(-1),
								SrcAddressDispersionThreshold(-1),
								DstAddressDispersionThreshold(-1),
								Mode(MODE_SERVER)
{
}

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

void CPacketManip::Initialize (int pargc, const char *pdev, const char *pfilter, const char *pServerPort, const char *pClientPort, const char *pServerIP)
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
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		cerr << "WSAStartup failed." << endl;
		exit(1);
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

	// Default server mode.
	if (pargc == 1 || pargc == 4)
	{
		int ServerPort;
		if (pargc == 1)
		{
			ServerPort = SERVERPORT;
			cout << "Running as Server and listening on default port " << ServerPort << endl;
		}
		else
		{
			ServerPort = atoi (pServerPort);
			cout << "Running as Server and listening on port " << ServerPort << endl;
		}

		// Server socket.
		SafeCall (SocketFD = socket (AF_INET, SOCK_DGRAM, 0), "socket()");
		// Server address initialization for binding.
		ServerAddress.sin_family = AF_INET;				// Socekt family.
		ServerAddress.sin_addr.s_addr = INADDR_ANY;		// Setting server IP. INADDR_ANY is the localhost IP.
		ServerAddress.sin_port = htons (ServerPort);	// Setting server port.
		fill ((char*)&(ServerAddress.sin_zero), (char*)&(ServerAddress.sin_zero)+8, '\0');

		// bind()
		SafeCall (bind (SocketFD, (sockaddr *)&ServerAddress, sizeof (ServerAddress)), "bind()");
		/*
		// recvfrom() is blocking and will wait for any messages from client.
		socklen_t ClientAddressSize = sizeof (ClientAddress);
		NumOfBytesReceived = recvfrom (ServerSocketFD, Buffer, MAXBUFFERSIZE-1, 0, (sockaddr *)&ClientAddress, &ClientAddressSize);

		Buffer[NumOfBytesReceived] = '\0';
		cout << "Server got packet from " << inet_ntoa (ClientAddress.sin_addr) << " on socket " << ServerSocketFD << endl;
		cout << "Client says: " << Buffer << endl;

		// sendto()
		char ServerMessage[] = "Hello from Server. Now bye!";
		NumOfBytesSent = sendto (ServerSocketFD, ServerMessage, strlen (ServerMessage), 0, (sockaddr *)&ClientAddress, sizeof (ClientAddress));*/
	}
	// If running in client mode.
	else if (pargc == 6)
	{
		// TODO: Set client mode.
		PacketCapture.Set_Mode (MODE_CLIENT);

		// Getting server's name/IP.
		if ((he = gethostbyname (pServerIP)) == NULL)
		{
			cerr << "ERROR: Cannot determine Server name/IP. " << endl;
			exit(-1);
		}
		// Creating a socket for the client.
		SafeCall (SocketFD = socket ( AF_INET, SOCK_DGRAM, 0 ), "socket()");

		// Initializing Client address for binding.
		ClientAddress.sin_family = AF_INET;							// Socket family.
		ClientAddress.sin_addr.s_addr = INADDR_ANY;					// Assigninig client IP.
		ClientAddress.sin_port = htons (atoi (pClientPort));		// Client port.
		fill ((char*)&(ClientAddress.sin_zero), (char*)&(ClientAddress.sin_zero)+8, '\0');

		// bind()
		SafeCall (bind (SocketFD, (sockaddr *)&ClientAddress, sizeof (ClientAddress)), "bind()");

		// Initializing Server address to connect to.
		ServerAddress.sin_family = AF_INET;							// Socket family.
		ServerAddress.sin_addr = *((in_addr *)(*he).h_addr);	// Server name/IP.
		ServerAddress.sin_port = htons (atoi (pServerPort));			// Server port provided as argument.
		fill ((char*)&(ServerAddress.sin_zero), (char*)&(ServerAddress.sin_zero)+8, '\0');

		/*
		// sendto()
		char ClientMessage[] = "Hello from client.";
		NumOfBytesSent = sendto (ClientSocketFD, ClientMessage, strlen (ClientMessage), 0, (sockaddr *)&ServerAddress, sizeof (ServerAddress));

		// recvfrom() is blocking and will wait for any messages.
		socklen_t ServerAddressSize = sizeof (ServerAddress);
		NumOfBytesReceived = recvfrom (ClientSocketFD, Buffer, MAXBUFFERSIZE-1, 0, (sockaddr *)&ServerAddress, &ServerAddressSize);		// Blocking.

		Buffer[NumOfBytesReceived] = '\0';
		cout << "Server says: " << Buffer << endl;*/
	}

	// Initializing thresholds.
	SetContentPrevalenceThreshold (20);
	SetSrcAddressDispersionThreshold (40);
	SetDstAddressDispersionThreshold (40);

	// Create Garbage Collector and Receiver threads.
	if (PacketCapture.Get_Mode() == MODE_SERVER)
	{
		#ifdef WIN32
		tGarbageCollector = (HANDLE)_beginthread (GarbageCollector, 0, NULL);
		tReceiver = (HANDLE)_beginthread (Receiver, 0, NULL);
		tLogger = (HANDLE)_beginthread (Logger, 0, NULL);
		#else
		pthread_create (&tGarbageCollector, NULL, GarbageCollector, NULL);
		pthread_create (&tReceiver, NULL, Receiver, NULL);
		pthread_create (&tLogger, NULL, Logger, NULL);
		#endif
	}
}
void CPacketManip::Loop ()
{
	pcap_loop (descr, -1, packet_capture_callback, NULL);
}

// Generate Key (sha1 hash) from protocol, destination port and payload.
unsigned char * CPacketManip::GenerateKey (unsigned char pip_p, unsigned short pth_dport, unsigned char *ppayload, unsigned int psize_payload)
{
	unsigned char *Key = new unsigned char[KEY_LENGTH];
	unsigned char *Input = new unsigned char[psize_payload + 3];
	Input[0] = pip_p;
	Input[1] = *(unsigned char*)(&pth_dport);
	Input[2] = *((unsigned char*)(&pth_dport)+1);
	for (unsigned int i=0; i<psize_payload; i++)
		Input[3+i] = ppayload[i];

	SHA1(Input, psize_payload+3, Key);
	return Key;
}

// Search Key in Content Prevalence Table.
int CPacketManip::SearchContentPrevalenceTable (unsigned char *pKey)
{
	for (int i=0; i < (int)ContentPrevalenceTable.size(); i++)
	{
		if (memncmp ((const char *)ContentPrevalenceTable[i].Key, (const char *)pKey, KEY_LENGTH) == true)
			return i;
	}
	return -1;
}

// Search Key in Address Dispersion Table.
int CPacketManip::SearchAddressDispersionTable (unsigned char *pKey)
{
	for (int i=0; i < (int)AddressDispersionTable.size(); i++)
	{
		if (memncmp ((const char *)AddressDispersionTable[i].Key, (const char *)pKey, KEY_LENGTH) == true)
			return i;
	}
	return -1;
}


// Search Src IP vector for a match.
bool CPacketManip::SearchSrcIPs (int SearchIndex, in_addr ip_src)
{
	for (int i=0; i < (int)AddressDispersionTable[SearchIndex].SrcIPs.size(); i++)
	{
		if (memncmp((const char *)&AddressDispersionTable[SearchIndex].SrcIPs[i], (const char *)&ip_src, sizeof(in_addr)) == true)
			return true;
	}
	return false;
}

// Search Dst IP vector for a match.
bool CPacketManip::SearchDstIPs (int SearchIndex, in_addr ip_dst)
{
	for (int i=0; i < (int)AddressDispersionTable[SearchIndex].DstIPs.size(); i++)
	{
		if (memncmp((const char *)&AddressDispersionTable[SearchIndex].DstIPs[i], (const char *)&ip_dst, sizeof(in_addr)) == true)
			return true;
	}
	return false;
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

		// Displaying payload data.
		if (size_payload != 0)
		{
			cout << "Payload: " << endl;
			//u_int Payload_Offset = SIZE_ETHERNET + size_ip + size_tcp;
			print_payload (payload/*packet+Payload_Offset*/, size_payload);
			/*
			for (int i=0; i<size_payload; i++)
			{
				cout << packet[i + Payload_Offset];
			}
			cout << endl;
			*/
		}
		cout << "*************************" << endl;

		// Key generation. Only generate signatures for packets with non-zero payload.
		if (size_payload != 0)
		{
			unsigned char *GeneratedKey = PacketCapture.GenerateKey (ip->ip_p, tcp->th_dport, (unsigned char *)payload, size_payload);

			// Printing key.
			cout << "Key: ";
			for (int i=0; i<KEY_LENGTH; i++)
			{
				cout << hex << setfill('0') << setw(2) << (int)GeneratedKey[i] << dec << " ";
			}
			cout << endl;
			// If this is Server then process the packet. Otherwise send signature data to Server.
			if (PacketCapture.Get_Mode() == MODE_SERVER)
			{
				#ifdef WIN32
				WaitForSingleObject (MutexLock, INFINITE);
				ProcessPacket (GeneratedKey, tcp->th_sport, tcp->th_dport, ip->ip_src, ip->ip_dst);
				ReleaseMutex (MutexLock);
				#else
				pthread_mutex_lock (&MutexLock);
				ProcessPacket (GeneratedKey, tcp->th_sport, tcp->th_dport, ip->ip_src, ip->ip_dst);
				pthread_mutex_unlock (&MutexLock);
				#endif
			}
			else
			{
				SignatureData temp;
				memncpy ((char *)temp.Key, (const char *)GeneratedKey, KEY_LENGTH);
				temp.th_sport = tcp->th_sport;
				temp.th_dport = tcp->th_dport;
				memncpy ((char *)&temp.ip_src, (const char *)&ip->ip_src, sizeof(in_addr));
				memncpy ((char *)&temp.ip_dst, (const char *)&ip->ip_dst, sizeof(in_addr));
				SafeCallAssert (NumOfBytesSent = sendto (SocketFD, (char *)&temp, sizeof(SignatureData), 0, (sockaddr *)&ServerAddress, sizeof (ServerAddress)), "sendto()", sizeof(SignatureData));
			}
		}
	}
	else
		return;		// Not a packet of our interest.
}

// Only Server will process packets.
void ProcessPacket (unsigned char *GeneratedKey, unsigned short th_sport, unsigned short th_dport, in_addr ip_src, in_addr ip_dst)
{
	int SearchIndex;
	// Check key entry in Address Dispersion Table.
	if ( (SearchIndex = PacketCapture.SearchAddressDispersionTable (GeneratedKey)) != -1)
	{
		cout << "Key already exists in Address Dispersion Table..." << endl;
		if (PacketCapture.SearchSrcIPs (SearchIndex, ip_src) == false)
			PacketCapture.AddressDispersionTable[SearchIndex].SrcIPs.push_back (ip_src);
		if (PacketCapture.SearchDstIPs (SearchIndex, ip_dst) == false)
			PacketCapture.AddressDispersionTable[SearchIndex].DstIPs.push_back (ip_dst);

		// Checking alarm thresholds.
		if (PacketCapture.AddressDispersionTable[SearchIndex].SrcIPs.size() > PacketCapture.GetSrcAddressDispersionThreshold() && PacketCapture.AddressDispersionTable[SearchIndex].DstIPs.size() > PacketCapture.GetDstAddressDispersionThreshold())
		{
			fstream AlarmLog("Alarm.log", std::ios::out | std::ios::app);
			AlarmLog << "######################## Alarm ########################" << endl;

			time_t rawtime;
			time ( &rawtime );
			AlarmLog << "Date/Time: " << asctime (localtime (&rawtime)) << endl;

			AlarmLog << "Key: ";
			for (int i=0; i<KEY_LENGTH; i++)
				AlarmLog << hex << setfill('0') << setw(2) << (int)PacketCapture.AddressDispersionTable[SearchIndex].Key[i] << dec << (i==9 ? " " : "");

			AlarmLog << "SrcIPCount = " << PacketCapture.AddressDispersionTable[SearchIndex].SrcIPs.size() << ", DstIPCount = " << PacketCapture.AddressDispersionTable[SearchIndex].DstIPs.size() << endl;
			AlarmLog << "Src Port = " << ntohs(th_sport) << ", Dst Port = " << ntohs(th_dport) << endl;
			AlarmLog.close();
		}
	}
	// Check Content Prevalence Table.
	else if ( (SearchIndex = PacketCapture.SearchContentPrevalenceTable (GeneratedKey)) == -1)
	{
		ContentPrevalenceEntry temp;
		memncpy ((char *)temp.Key, (const char *)GeneratedKey, KEY_LENGTH);
		temp.Count = 1;
		temp.InsertionTime = GetTimeus64();
		PacketCapture.ContentPrevalenceTable.push_back(temp);
	}
	else
	{
		PacketCapture.ContentPrevalenceTable[SearchIndex].Count++;
		PacketCapture.ContentPrevalenceTable[SearchIndex].InsertionTime = GetTimeus64();
		if (PacketCapture.ContentPrevalenceTable[SearchIndex].Count > PacketCapture.GetContentPrevalenceThreshold())
		{
			// Check threshold and promote entry into address dispersion table.
			AddressDispersionEntry temp;
			memncpy ((char *)temp.Key, (const char *)GeneratedKey, KEY_LENGTH);
			temp.SrcIPs.push_back (ip_src);
			temp.DstIPs.push_back (ip_dst);

			// Insert into Address Dispersion table.
			PacketCapture.AddressDispersionTable.push_back (temp);
			// Erase from Content Prevalence Table.
			PacketCapture.ContentPrevalenceTable.erase (PacketCapture.ContentPrevalenceTable.begin() + SearchIndex);
		}
	}
}

// Compare two blocks of memory and returns true if they match else returns false.
bool memncmp (const char *block1, const char *block2, int size)
{
	for (int i=0; i<size; i++)
	{
		if (block1[i] != block2[i])
			return false;
	}
	return true;
}

void memncpy (char *dst, const char *src, int size)
{
	for (int i=0; i<size; i++)
		dst[i] = src[i];
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

void SafeCall (int returnvalue, const char *FunctionName, int errorvalue)
{
	if (returnvalue == errorvalue)
	{
		cerr << "ERROR: " << FunctionName << endl;
		exit (-1);
	}
}

void SafeCallAssert (int returnvalue, const char *FunctionName, int expectedvalue)
{
	if (returnvalue != expectedvalue)
	{
		cerr << "ERROR: " << FunctionName << endl;
		exit (-1);
	}
}

// Threads.
THREAD_RETURN_TYPE GarbageCollector (void *arg)
{
	__int64 Start = GetTimeus64();
	__int64 Current = GetTimeus64();
	while (true)
	{
		#ifdef WIN32
		Sleep (10*1000);
		#else
		sleep (10);
		#endif

		Current = GetTimeus64();
		cout << "Elapsed time = " << ((double)(Current-Start))/(1000000.) << " seconds." << endl;
		cout << "Garbage collection started..." << endl;

		//  Garbage Collection.
		#ifdef WIN32
		WaitForSingleObject (MutexLock, INFINITE);
		#else
		pthread_mutex_lock (&MutexLock);
		#endif
		for (int i=0; i < (int)PacketCapture.ContentPrevalenceTable.size(); i++)
		{
			// Check if an entry has persisted for 15 seconds without an increase in prevalence count then decrease its prevalence count.
			if (((double)(Current-PacketCapture.ContentPrevalenceTable[i].InsertionTime))/(1000000.) > 15.)
			{
				// Decrease prevalence count.
				PacketCapture.ContentPrevalenceTable[i].Count--;

				// If Count is zero then erase the entry.
				if (PacketCapture.ContentPrevalenceTable[i].Count == 0)
					PacketCapture.ContentPrevalenceTable.erase (PacketCapture.ContentPrevalenceTable.begin()+i);
			}
		}
		#ifdef WIN32
		ReleaseMutex (MutexLock);
		#else
		pthread_mutex_unlock (&MutexLock);
		#endif
	}
	#ifndef WIN32
	return NULL;
	#endif
}

THREAD_RETURN_TYPE Receiver (void *arg)
{
	SignatureData temp;
	while (true)
	{
		sin_size = sizeof (ClientAddress);
		SafeCallAssert (NumOfBytesReceived = recvfrom (SocketFD, (char *)&temp, sizeof(SignatureData), 0, (sockaddr *)&ClientAddress, &sin_size), "recvfrom()", sizeof(SignatureData));

		#ifdef WIN32
		WaitForSingleObject (MutexLock, INFINITE);
		ProcessPacket (temp.Key, temp.th_sport, temp.th_dport, temp.ip_src, temp.ip_dst);
		ReleaseMutex (MutexLock);
		#else
		pthread_mutex_lock (&MutexLock);
		ProcessPacket (temp.Key, temp.th_sport, temp.th_dport, temp.ip_src, temp.ip_dst);
		pthread_mutex_unlock (&MutexLock);
		#endif
	}
	#ifndef WIN32
	return NULL;
	#endif
}

// Tables are logged every 5 seconds.
THREAD_RETURN_TYPE Logger (void *arg)
{
	__int64 Start = GetTimeus64();
	__int64 Current = GetTimeus64();
	while (true)
	{
		#ifdef WIN32
		Sleep (5*1000);
		#else
		sleep (5);
		#endif

		Current = GetTimeus64();
		cout << "Elapsed time = " << ((double)(Current-Start))/(1000000.) << " seconds." << endl;
		cout << "Writing logs..." << endl;

		//  Garbage Collection.
		#ifdef WIN32
		WaitForSingleObject (MutexLock, INFINITE);
		#else
		pthread_mutex_lock (&MutexLock);
		#endif
		// Writing logs.
		fstream ContentPrevalenceTableLog("ContentPrevalenceTable.log", std::ios::out);
		for (int i=0; i < (int)PacketCapture.ContentPrevalenceTable.size(); i++)
		{
			for (int j=0; j<KEY_LENGTH; j++)
				ContentPrevalenceTableLog << hex << setfill('0') << setw(2) << (int)PacketCapture.ContentPrevalenceTable[i].Key[j] << dec << (j==9 ? " " : "");
			ContentPrevalenceTableLog << ": " << PacketCapture.ContentPrevalenceTable[i].Count << endl;
		}
		ContentPrevalenceTableLog.close();

		fstream AddressDispersionTableLog("AddressDispersionTable.log", std::ios::out);
		for (int i=0; i < (int)PacketCapture.AddressDispersionTable.size(); i++)
		{
			for (int j=0; j<KEY_LENGTH; j++)
				AddressDispersionTableLog << hex << setfill('0') << setw(2) << (int)PacketCapture.AddressDispersionTable[i].Key[j] << dec << (j==9 ? " " : "");
			AddressDispersionTableLog << ": SrcIPCount=" << PacketCapture.AddressDispersionTable[i].SrcIPs.size() << ", DstIPCount=" << PacketCapture.AddressDispersionTable[i].DstIPs.size() << endl;
		}
		AddressDispersionTableLog.close();
		#ifdef WIN32
		ReleaseMutex (MutexLock);
		#else
		pthread_mutex_unlock (&MutexLock);
		#endif
	}
	#ifndef WIN32
	return NULL;
	#endif
}
