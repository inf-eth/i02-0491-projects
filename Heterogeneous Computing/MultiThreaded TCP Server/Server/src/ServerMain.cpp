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
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

TRET_TYPE AcceptorThread (void *);
TRET_TYPE ClientHandlerThread (void *);

#if defined __linux__ || defined __CYGWIN__
pthread_mutex_t PlatformMutexLock = PTHREAD_MUTEX_INITIALIZER;
#else
HANDLE PlatformMutexLock = CreateMutex (NULL, FALSE, NULL);
#endif

int main (int argc, char *argv[])
{
	#if defined __linux__ || defined __CYGWIN__
	std::cout << "Running on linux..." << std::endl;
	pthread_t* Acceptorth = new pthread_t;
	#else
	std::cout << "Running on windows..." << std::endl;
	HANDLE* Acceptorth = new HANDLE;
	#endif

	// Create Server object.
	CServer ServerObj (TCPSOCKET, DEFAULTSERVERPORT);			// Without any arguments Server will set defaults, TCPSOCKET as type and 6000 as port.

	// Create Server socket and set socket options.
	ServerObj.CreateSocket (TCPSOCKET);		// No argument means TCPSOCKET
	ServerObj.SetSocketOptions ();

	// Initial Server address struct and bind it with Server's socket.
	ServerObj.InitializeAddress (6000);		// No argument here will take default port.
	ServerObj.Bind ();

	// Listen for incoming connections.
	ServerObj.Listen ();

	#if defined __linux__ || defined __CYGWIN__
	pthread_create (Acceptorth, NULL, AcceptorThread, (void*)&ServerObj);
	#else
	*Acceptorth = (HANDLE)_beginthread (AcceptorThread, 0, (void*)&ServerObj);
	#endif

	while (true)
	{
		char choice;
		cout << "=======================================" << endl;
		cout << "1. Display server info." << endl;
		cout << "2. Display client info of all connections." << endl;
		cout << "3. Display client info (by index)." << endl;
		cout << "4. Start computation on all connected clients." << endl;
		cout << "E. Exit." << endl;
		cout << ">>";
		cin >> choice;

		switch (choice)
		{
			case '1':
			{
				ServerObj.DisplayServerInfo ();
				break;
			}
			case '2':
			{
				ServerObj.DisplayAllClientsInfo();
				break;
			}
			case '3':
			{
				int Index;
				cout << "Enter index: ";
				cin >> Index;
				ServerObj.DisplayClientInfo(Index);
				break;
			}
			case '4':
			{
				break;
			}
			case 'E':
			{
				// Close sockets.
				ServerObj.CloseAllClientSockets();
				ServerObj.CloseServerSocket();
				return 0;
				break;
			}
		}
	}
	// Send and receive.
	ServerObj.Receive ();
	cout << "No. of bytes received: " << ServerObj.GetNumOfBytesReceived () << endl;
	cout << "Client says: " << ServerObj.GetBuffer() << endl;

	char message[] = "Hello from server.";
	ServerObj.Send ((void *)message, (unsigned int)strlen (message));

	return 0;
}

TRET_TYPE AcceptorThread (void *ServerPTR)
{
	while (true) // infinite loop.
	{
		// Accept any incoming connections.
		int ClientIndex = ((CServer*)ServerPTR)->AcceptClient();
	}

#ifndef WIN32
	return NULL;
#endif
}

TRET_TYPE ClientHandlerThread (void *ServerPTR)
{
	while (true) // infinite loop.
	{
		// Accept any incoming connections.
		int ClientIndex = ((CServer*)ServerPTR)->Accept ();
		((CServer*)ServerPTR)->DisplayClientInfo (ClientIndex);
	}

#ifndef WIN32
	return NULL;
#endif
}
