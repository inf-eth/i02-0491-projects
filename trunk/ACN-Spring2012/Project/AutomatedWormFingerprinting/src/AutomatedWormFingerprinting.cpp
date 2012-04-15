/*
# AutomatedWormFingerprinting.cpp
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
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################
*/

#include <PacketManip.h>
#include <cstring>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

extern CPacketManip PacketCapture;

// Arguments required:
// Can provide none, 3 or 5 additional arguments.
// No arguments: Default capture device, filter "tcp||udp", 6011 server port. Operating in server mode.
// 3 arguments: Specified capture device, filter and server port. Operating in server mode.
// 5 arguments: Specified capture device, filter, server port, client port and server IP. Operating in client mode.

// 1. Device to sniff on: e.g. "eth0", 'wlan0". Can be passed as 'NULL' for autoselection.
// 2. Filter string: See examples.
// Mode of operation depends on number of arguments: Server = 3, Client = 5
// 3. Server port
// 4. Client port (Client mode only)
// 5. Server IP (Client mode only)

int main (int argc, char **argv)
{
	cout << "Automated Worm Fingerprinting." << endl;

	// Empty filter program will capture all packets on device.
	// set device as "lo" to capture packets on loopback interface.
	// filter program can be "dst localhost", "src localhost", "dst www.google.com" etc.
	
	cout << "argc = " <<  argc << endl;

	if (argc == 1)
		PacketCapture.Initialize (argc, NULL, "tcp || udp", NULL, NULL, NULL);
	else if (argc == 4)
		PacketCapture.Initialize (argc, !strcmp(argv[1], "NULL") ? NULL : argv[1], argv[2], argv[3], NULL, NULL);
	else if (argc == 6)
		PacketCapture.Initialize (argc, !strcmp(argv[1], "NULL") ? NULL : argv[1], argv[2], argv[3], argv[4], argv[5]);
	else
	{
		cerr << "ERROR: Invalid argument list." << endl;
		cout << "Input format: " << endl
			 << "Arguments required: " << endl
			 << "Can provide none, 3 or 5 additional arguments." << endl
			 << "No arguments: Default capture device, filter 'tcp||udp', 6011 server port. Operating in server mode." << endl
			 << "3 arguments:  Specified capture device, filter and server port. Operating in server mode." << endl
			 << "5 arguments:  Specified capture device, filter, server port, client port and server IP. Operating in client mode." << endl
			 << endl
			 << "1. Device to sniff on: e.g. eth0, wlan0. Can be 'NULL' for autoselection." << endl
			 << "2. Filter string: See examples." << endl
			 << "Mode of operation depends on number of arguments: Server = 3, Client = 5" << endl
			 << "3. Server port" << endl
			 << "4. Client port (Client mode only)" << endl
			 << "5. Server IP (Client mode only)" << endl;
		return -1;
	}
	/*
	if (argc == 3)
		PacketCapture.Initialize (argv[1], argv[2]);
	else
		PacketCapture.Initialize (NULL, "tcp || udp");
	*/
	PacketCapture.Loop ();
	return 0;
}
