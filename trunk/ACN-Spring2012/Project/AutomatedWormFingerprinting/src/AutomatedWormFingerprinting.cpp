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

	// Check if user passed capture device and filter string as argument.
	if (argc == 3)
		PacketCapture.Initialize (argv[1], argv[2]);
	// Defulat is to capture all packets on eth0.
	else
		PacketCapture.Initialize ("eth0", "");

	PacketCapture.Loop ();
	return 0;
}