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
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################
*/

#include <PacketManip.h>
#include <iostream>
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
	cout << "Automated Worm Fingerprinting." << endl;

	CPacketManip PacketCapture;

	// Empty filter program will capture all packets on device.
	// set device as "lo" to capture packets on loopback interface.
	// filter program can be "dst localhost", "src localhost", "dst www.google.com" etc.
	cout << "argc = " <<  argc << endl;
	// Check if user passed capture device and filter string as argument.
	if (argc == 3)
		PacketCapture.Initialize (argv[1], argv[2]);
	// Default is to capture all ip packets on default interface.
	else
		PacketCapture.Initialize (NULL, "tcp || udp");

	PacketCapture.Loop ();
	return 0;
}
