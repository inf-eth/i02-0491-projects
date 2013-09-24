#include <Client.h>
#include <cstdlib>			// exit(), atoi()
#include <cstring>			// strlen()
#include <iostream>
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
	// Standard error checking. Must provide server name/IP and port to connect.
	if ( argc < 4 )
	{
		cout << "ERROR000: Incorrect input arguments." << endl;
		cout << "Usage: './Client [server name or IP] [server port] [client port]'" << endl;
		cout << "OR" << endl;
		cout << "if using Makefile: 'make runC ARG=\"[server name or IP] [server port] [client port]\"'" << endl;
		exit (-1);
	}

	int ClientPort = atoi(argv[3]);

	// Create Client object.
	CClient ClientObj (TCPSOCKET, ClientPort);			// Can create an object without arguments. Defaults are TCPSOCKET and DEFAULTPORT (6001).

	// Create Client Socket.
	ClientObj.CreateSocket (TCPSOCKET);
	ClientObj.SetSocketOptions ();

	// Initialize and bind Client address.
	ClientObj.InitializeAddress (ClientPort);	// Without any argument default port will be chosen.
	ClientObj.Bind ();

	// Connect to Server. Server name/IP and port are provided as arguments.
	ClientObj.Connect (argv[1], atoi (argv[2]));

	double Computation = 1.f/2.4f;
	const char ID[] = "C2D E8400 @3.00 GHz";
	// Send and receive.
	ClientObj.Send ((void *)&Computation, sizeof(double));

	ClientObj.Receive ();
	if (strncmp("ACK", ClientObj.GetBuffer(), 3U) != 0)
	{
		cout << "Error receiving acknowledgement from server." << endl;
		return -1;
	}

	ClientObj.Send ((void *)ID, sizeof(ID));

	ClientObj.CloseClientSocket ();

	/*
	// ********************* UDP Communication ***************************
	cout << "TCP connection closed, now doing UDP communication." << endl;
	ClientObj.CreateSocket (UDPSOCKET);
	ClientObj.InitializeAddress (6001);
	ClientObj.Bind ();

	char UDPmessage[] = "UDP message from client.";
	ClientObj.SendTo ((void *)UDPmessage, (unsigned int)strlen (UDPmessage), "localhost", 6000);

	cout << "Client recv'in from() packets..." << endl;
	ClientObj.RecvFrom ();
	cout << "They say: " << ClientObj.GetBuffer () << endl;
	ClientObj.DisplayTheirInfo ();

	ClientObj.CloseClientSocket ();
	// *******************************************************************
	*/

	return 0;
}
