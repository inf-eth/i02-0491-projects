/*
# PacketManip.h
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
#define HAVE_REMOTE
#include <PacketHeaders.h>
#include <rabinhash32.h>
#include <rabinhash64.h>
#include <pcap.h>
#include <iostream>
#include <vector>

using std::ostream;
using std::vector;

// HASHING_SCHEMES
// 0: Rabin 32bit
// 1: Rabin 32bit interleaved
// 2: Rabin 64bit
// 3: Rabin 64bit interleaved
// 4: MD5
// 5: SHA1
// 6: SHA256

#define HASHING_SCHEME					1

#define LOG_NETWORK_STATS				1		// Log network statistics? 1=Yes, 0=No
#define LOG_PORT_NETWORK_STATS			1		// If logging network stats, also log port-based network statistics? 1=Yes, 0=No.
#define LOG_HOST_NETWORK_STATS			1		// If logging network stats, also log Host-based network statistics? 1=Yes, 0=No.
#define LOG_HOST_PORT_NETWORK_STATS		1		// If logging host and port stats, also log port-based network statistics for each host? 1=Yes, 0=No.

#define SUBSTRING_PROCESSING			1
#define SUBSTRING_WINDOW				30
#define CONTENT_PREVALENCE_TIMEOUT		120.	// seconds
#define CONTENT_PREVALENCE_THRESHOLD	20
#define SRC_IP_DISPERSION_THRESHOLD		5
#define DST_IP_DISPERSION_THRESHOLD		5
#define GARBAGE_COLLECTION_INTERVAL		60		// seconds
#define LOGGING_INTERVAL				30		// seconds

#if HASHING_SCHEME == 0
#define HASHING_METHOD	RABIN32
#define KEY_LENGTH		4
#define RABIN_POLY		0x4D96487B
#endif
#if HASHING_SCHEME == 1
#define HASHING_METHOD	RABIN32_INTERLEAVED
#define KEY_LENGTH		8
#define RABIN_POLY1		0x4D96487B
#define RABIN_POLY2		0x2DC7EEB3
#endif
#if HASHING_SCHEME == 2
#define HASHING_METHOD	RABIN64
#define KEY_LENGTH		8
#define RABIN_POLY		0x460C880810028043
#endif
#if HASHING_SCHEME == 3
#define HASHING_METHOD	RABIN64_INTERLEAVED
#define KEY_LENGTH		16
#define RABIN_POLY1		0x460C880810028043
#define RABIN_POLY2		0x7523C013A96DD7FF
#endif
#if HASHING_SCHEME == 4
#define HASHING_METHOD	MD5
#define KEY_LENGTH		16
#endif
#if HASHING_SCHEME == 5
#define HASHING_METHOD	SHA1
#define KEY_LENGTH		20
#endif
#if HASHING_SCHEME == 6
#define HASHING_METHOD	SHA256
#define KEY_LENGTH		32
#endif

#ifndef WIN32
#define __int64 long long
#endif

enum MODE_OF_OPERATION
{
	MODE_SERVER = 0,
	MODE_CLIENT = 1
};

struct ContentPrevalenceEntry
{
	unsigned char Key[KEY_LENGTH];
	int Count;
	__int64 InsertionTime;
};

struct AddressDispersionEntry
{
	unsigned char Key[KEY_LENGTH];
	vector<in_addr> SrcIPs;
	vector<unsigned short> SrcPorts;
	vector<in_addr> DstIPs;
	vector<unsigned short> DstPorts;
	int AlarmCount;
};

#if LOG_NETWORK_STATS == 1
class CNetworkStats
{
public:
	long long tStart, tCurrent;
	unsigned long long TotalPackets;
	unsigned long long TCPPackets;
	unsigned long long UDPPackets;

	unsigned long long TotalTraffic;
	unsigned long long TotalTrafficPayload;
	unsigned long long TotalTrafficTCP;
	unsigned long long TotalTrafficTCPPayload;
	unsigned long long TotalTrafficUDP;
	unsigned long long TotalTrafficUDPPayload;

	CNetworkStats();
	void UpdateStats(long long, unsigned char, unsigned short, unsigned short, in_addr, in_addr, unsigned int, unsigned int);
	friend ostream& operator <<(ostream &, const CNetworkStats &);
};

#if LOG_PORT_NETWORK_STATS == 1
class PortNetworkStats
{
public:
	CNetworkStats NetworkStats;
	unsigned short Port;
};
#endif

#if LOG_HOST_NETWORK_STATS == 1
class HostNetworkStats
{
public:
	CNetworkStats NetworkStats;
	in_addr IP;
	#if LOG_PORT_NETWORK_STATS == 1 && LOG_HOST_NETWORK_STATS == 1 && LOG_HOST_PORT_NETWORK_STATS == 1
	vector<PortNetworkStats> PortsNetworkStats;
	void UpdatePortsNetworkStats(long long, unsigned char, unsigned short, unsigned short, in_addr, in_addr, unsigned int, unsigned int);
	#endif
};
#endif

#endif

// Packet capturing and manipulation class.
class CPacketManip
{
private:
	char *dev;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* descr;
	const u_char *packet;
	struct pcap_pkthdr hdr;     /* pcap.h                    */
	struct ether_header *eptr;  /* net/ethernet.h            */
	struct bpf_program fp;      /* hold compiled program     */
	bpf_u_int32 maskp;          /* subnet mask               */
	bpf_u_int32 netp;           /* ip                        */

	// Thresholds.
	int ContentPrevalenceThreshold;
	int SrcAddressDispersionThreshold;
	int DstAddressDispersionThreshold;

	// Rabin Schemes.
#if HASHING_SCHEME == 0
	RabinHashFunction32 rabin32;
#endif
#if HASHING_SCHEME == 1
	RabinHashFunction32 rabin32_1;
	RabinHashFunction32 rabin32_2;
#endif
#if HASHING_SCHEME == 2
	RabinHashFunction64 rabin64;
#endif
#if HASHING_SCHEME == 3
	RabinHashFunction64 rabin64_1;
	RabinHashFunction64 rabin64_2;
#endif

	// Working as server or client?
	MODE_OF_OPERATION Mode;		// Default mode is MODE_SERVER.
	
public:
	CPacketManip ();
	CPacketManip (char *, char *);					// takes device name and filter program as arguments.
	void Initialize (int ,const char*, const char *, const char *, const char *, const char *);	// parse arguments
	void Loop ();

	void Set_Mode(MODE_OF_OPERATION pMode) { Mode = pMode; }
	MODE_OF_OPERATION Get_Mode() { return Mode; }
	
	// Set Thresholds.
	void SetContentPrevalenceThreshold (int pThreshold) { ContentPrevalenceThreshold = pThreshold; }
	void SetSrcAddressDispersionThreshold (int pThreshold) { SrcAddressDispersionThreshold = pThreshold; }
	void SetDstAddressDispersionThreshold (int pThreshold) { DstAddressDispersionThreshold = pThreshold; }

	// Get Thresholds.
	int GetContentPrevalenceThreshold () { return ContentPrevalenceThreshold; }
	int GetSrcAddressDispersionThreshold () { return SrcAddressDispersionThreshold; }
	int GetDstAddressDispersionThreshold () { return DstAddressDispersionThreshold; }

	// Content Prevalence Table
	vector<ContentPrevalenceEntry> ContentPrevalenceTable;
	// Address Dispersion Table
	vector<AddressDispersionEntry> AddressDispersionTable;

	// Generate Key (sha1 hash) from protocol, destination port and payload.
	unsigned char * GenerateKey (unsigned char *, unsigned int);

	// Search Key in Content Prevalence Table.
	int SearchContentPrevalenceTable (unsigned char *);

	// Search Key in Address Dispersion Table.
	int SearchAddressDispersionTable (unsigned char *);

	// Search Src IPs for a match.
	bool SearchSrcIPs (int, in_addr);
	// Search Dst IPs for a match.
	bool SearchDstIPs (int, in_addr);

	// Stats logging.
	#if LOG_NETWORK_STATS == 1
	CNetworkStats NetworkStats;

	#if LOG_PORT_NETWORK_STATS == 1
	vector<PortNetworkStats> PortsNetworkStats;
	void UpdatePortsNetworkStats(long long, unsigned char, unsigned short, unsigned short, in_addr, in_addr, unsigned int, unsigned int);
	#endif

	#if LOG_HOST_NETWORK_STATS == 1
	vector<HostNetworkStats> HostsNetworkStats;
	void UpdateHostsNetworkStats(long long, unsigned char, unsigned short, unsigned short, in_addr, in_addr, unsigned int, unsigned int);
	#endif

	#endif

};

// Compare two blocks of memory and returns true if they match else returns false.
bool memncmp (const char *, const char *, int);		// Byte stream comparison
void memncpy (char *, const char *, int);			// Byte stream copy
bool memncmp32 (const char *, const char *, int);	// 32bit word stream comparison
void memncpy32 (const char *, const char *, int);	// 32bit word stream copy
bool memncmp64 (const char *, const char *, int);	// 64bit word stream comparison
void memncpy64 (const char *, const char *, int);	// 64bit word stream copy

void print_hex_ascii_line(const u_char *, int, int);
void print_payload(const u_char *, int);

void SafeCall (int, const char *, int=-1);
void SafeCallAssert (int, const char *, int=0);
