#if defined __linux__ || defined __CYGWIN__
#include <pthread.h>
#define TRET_TYPE void*
#include <sys/time.h>
#else
#include <process.h>
#define TRET_TYPE void
#endif

#include <Client.h>
#include <cstdlib>			// exit(), atoi()
#include <cstring>			// strlen()
#include <string>
#include <iostream>
using std::cin;
using std::string;
using std::cout;
using std::endl;

TRET_TYPE SenderThread (void *);
TRET_TYPE ReceiverThread (void *);

int main (int argc, char **argv)
{
	// Standard error checking. Must provide server name/IP and port to connect.
	if ( argc < 3 )
	{
		cout << "ERROR000: Incorrect input arguments." << endl;
		cout << "Usage: './Client [server name or IP] [server port]'" << endl;
		cout << "OR" << endl;
		cout << "if using Makefile: 'make runC ARG=\"[server name or IP] [server port]\"'" << endl;
		exit (-1);
	}

	#if defined __linux__ || defined __CYGWIN__
	std::cout << "Running on linux..." << std::endl;
	pthread_t* threads = new pthread_t[2];
	#else
	std::cout << "Running on windows..." << std::endl;
	HANDLE* th = new HANDLE[2];
	#endif

	// Create Client object.
	Client ClientObj (TCPSOCKET, DEFAULTCLIENTPORT);			// Can create an object without arguments. Defaults are TCPSOCKET and DEFAULTPORT (6001).

	// Create Client Socket.
	ClientObj.CreateSocket (TCPSOCKET);
	ClientObj.SetSocketOptions ();

	// Initialize and bind Client address.
	ClientObj.InitialiseAddress (DEFAULTCLIENTPORT);	// Without any argument default port will be chosen.
	ClientObj.Bind ();

	// Connect to Server. Server name/IP and port are provided as arguments.
	ClientObj.Connect (argv[1], atoi (argv[2]));

	// Start Sender and receiver threads.
	#if defined __linux__ || defined __CYGWIN__
	pthread_create (&threads[0], NULL, SenderThread, (void*)&ClientObj);
	pthread_create (&threads[1], NULL, ReceiverThread, (void*)&ClientObj);
	#else
	th[0] = (HANDLE)_beginthread (SenderThread, 0, (void*)&ClientObj);
	th[1] = (HANDLE)_beginthread (ReceiverThread, 0, (void*)&ClientObj);
	#endif

	// Wait on threads.
	for (int i=0; i<2; i++)
	{
		#if defined __linux__ || defined __CYGWIN__
		pthread_join (threads[i], NULL);
		#else
		WaitForSingleObject(th[i], INFINITE);
		#endif
	}

	ClientObj.CloseClientSocket ();

	return 0;
}

// Takes input string and sends to server.
TRET_TYPE SenderThread (void *ClientPTR)
{
	while (true) // infinite loop.
	{
		string ClientMessage;
		cout << ">>";
		getline(cin, ClientMessage);
		((Client*)ClientPTR)->Send ((void *)ClientMessage.c_str(), (unsigned int)strlen (ClientMessage.c_str()));
	}

#ifndef WIN32
	return NULL;
#endif
}

TRET_TYPE ReceiverThread (void *ClientPTR)
{
	while (true) // infinite loop.
	{
		((Client*)ClientPTR)->Receive (); // Blocking.
		cout << "Server says: " << ((Client*)ClientPTR)->GetBuffer () << endl;
	}

#ifndef WIN32
	return NULL;
#endif
}
