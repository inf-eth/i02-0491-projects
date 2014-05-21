#if defined __linux__ || defined __CYGWIN__
#include <pthread.h>
#define TRET_TYPE void*
#include <sys/time.h>
#else
#include <process.h>
#define TRET_TYPE void
#endif

#include <Server.h>
#include <cstring>
#include <string>
#include <iostream>
using std::cin;
using std::string;
using std::cout;
using std::endl;

TRET_TYPE SenderThread (void *);
TRET_TYPE ReceiverThread (void *);

int main (int argc, char *argv[])
{
	#if defined __linux__ || defined __CYGWIN__
	std::cout << "Running on linux..." << std::endl;
	pthread_t* threads = new pthread_t[2];
	#else
	std::cout << "Running on windows..." << std::endl;
	HANDLE* th = new HANDLE[2];
	#endif

	// Create Server object.
	Server ServerObj (UDPSOCKET, DEFAULTSERVERPORT);			// Without any arguments Server will set defaults, TCPSOCKET as type and 6000 as port.

	ServerObj.CreateSocket (UDPSOCKET);
	ServerObj.InitialiseAddress (6000);
	ServerObj.Bind ();

	// Start Sender and receiver threads.
	#if defined __linux__ || defined __CYGWIN__
	pthread_create (&threads[0], NULL, SenderThread, (void*)&ServerObj);
	pthread_create (&threads[1], NULL, ReceiverThread, (void*)&ServerObj);
	#else
	th[0] = (HANDLE)_beginthread (SenderThread, 0, (void*)&ServerObj);
	th[1] = (HANDLE)_beginthread (ReceiverThread, 0, (void*)&ServerObj);
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

	// Close sockets.
	ServerObj.CloseServerSocket ();

	return 0;
}

// Takes input string and sends to server.
TRET_TYPE SenderThread (void *ServerPTR)
{
	while (true) // infinite loop.
	{
		string ServerMessage;
		cout << ">>";
		getline(cin, ServerMessage);
		//((Server*)ServerPTR)->SendTo ((void *)ServerMessage.c_str(), (unsigned int)strlen (ServerMessage.c_str())); // If sending a reply packet most recent received address is used.
		((Server*)ServerPTR)->SendTo ((void *)ServerMessage.c_str(), (unsigned int)strlen (ServerMessage.c_str()), "localhost", 6001);	// Need to explicitly mention receiver address if not a reply.
	}

#ifndef WIN32
	return NULL;
#endif
}

TRET_TYPE ReceiverThread (void *ServerPTR)
{
	while (true) // infinite loop.
	{
		((Server*)ServerPTR)->RecvFrom (); // Blocking.
		cout << "They say: " << ((Server*)ServerPTR)->GetBuffer () << endl;
		((Server*)ServerPTR)->DisplayTheirInfo ();
	}

#ifndef WIN32
	return NULL;
#endif
}
