#include <Server.h>
#include <cstring>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::fill;

CServer::CServer (int pType, int pServerPort): Type (pType), ServerPort (pServerPort)
{
	#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		cerr << "WSAStartup failed." << endl;
		exit(1);
	}
	#endif
	cout << "Server object created with type " << (Type == TCPSOCKET ? "TCP" : "UDP") << " and port " << ServerPort << endl;
	for (int i=0; i<MAXCLIENTS; i++)
		ClientSocketFD[i] = -1;
	Connections = 0;
}

int CServer::CreateSocket (int pType)			// 0 = TCP, 1 = UDP; default is to create TCP socket.
{
	int errorcheck;
	if (pType == TCPSOCKET)
	{
		Type = TCPSOCKET;
		errorcheck = ServerSocketFD = (int)socket (AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		Type = UDPSOCKET;
		errorcheck = ServerSocketFD = (int)socket (AF_INET, SOCK_DGRAM, 0);
	}
	if (errorcheck == -1)
	{
		cerr << "ERROR001: Creating socket of type " << pType << endl;
	}
	return errorcheck;
}

int CServer::SetSocketOptions ()					// Set socket options to reuse address.
{
	int Yes = 1;
	int errorcheck;
	errorcheck = setsockopt (ServerSocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&Yes, sizeof (int));
	if (errorcheck == -1)
	{
		cerr << "ERROR002: Setting socket options. " << endl;
	}
	return errorcheck;
}

int CServer::InitializeAddress (int pPort)	// Default Server port is 5000.
{
	ServerPort = pPort;
	// Server address initialization for binding.
	ServerAddress.sin_family = AF_INET;				// Socekt family.
	ServerAddress.sin_addr.s_addr = INADDR_ANY;		// Setting server IP. INADDR_ANY blank IP.
	ServerAddress.sin_port = htons (ServerPort);	// Setting server port.
	fill ((char*)&(ServerAddress.sin_zero), (char*)&(ServerAddress.sin_zero)+8, '\0');
	return 0;
}

int CServer::Bind ()								// Bind Server socket with address.
{
	int errorcheck;
	errorcheck = bind (ServerSocketFD, (sockaddr *)&ServerAddress, sizeof (ServerAddress));
	if (errorcheck == -1)
	{
		cerr << "ERROR003: Binding." << endl;
	}
	return errorcheck;
}

int CServer::Listen ()								// Listen for incoming connections; for TCP Server.
{
	int errorcheck;
	errorcheck = listen (ServerSocketFD, 0);
	if (errorcheck == -1)
	{
		cerr << "ERROR004: Listening." << endl;
	}
	return errorcheck;
}

// Returns index of connected client.
int CServer::Accept ()								// Accept incoming connections.
{
	socklen_t ClientAddressLength = sizeof (sockaddr_in);
	int FreeClientSocket = -1;
	if (Connections < MAXCLIENTS)
	{
		for (int i=0; i<MAXCLIENTS; i++)
		{
			if (ClientSocketFD[i] == -1)
			{
				FreeClientSocket = i;
				break;
			}
		}
		int errorcheck = ClientSocketFD[FreeClientSocket] = (int)accept (ServerSocketFD, (sockaddr *)&ClientAddress[FreeClientSocket], &ClientAddressLength);
		if (errorcheck == -1)
		{
			cerr << "ERROR005: Accepting." << endl;
			return errorcheck;
		}
		cout << "*** Server got connection from " << inet_ntoa (ClientAddress[FreeClientSocket].sin_addr) << " on socket '" << ClientSocketFD[FreeClientSocket] << "' ***" << endl;
	}
	return FreeClientSocket;
}

int CServer::Receive (int ClientIndex)
{
	if (ClientIndex < 0 || ClientIndex >= MAXCLIENTS)
	{
		cerr << "ERROR06A Client index out of bound." << endl;
		return -1;
	}
	if (ClientSocketFD[ClientIndex] == -1)
	{
		cerr << "ERROR06B Invalid client socket file descriptor number." << endl;
		return -1;
	}

	errorcheck[ClientIndex] = NumOfBytesReceived[ClientIndex] = recv (ClientSocketFD[ClientIndex], Buffer[ClientIndex], MAXBUFFERSIZE-1, 0);
	if (errorcheck[ClientIndex] == -1)
	{
		cerr << "ERROR006 Receiving" << endl;
		return errorcheck[ClientIndex];
	}
	Buffer[ClientIndex][NumOfBytesReceived[ClientIndex]] = '\0';
	return errorcheck[ClientIndex];
}

int CServer::Send (void *Data, unsigned int DataSize, int ClientIndex)
{
	if (ClientIndex < 0 || ClientIndex >= MAXCLIENTS)
	{
		cerr << "ERROR07A Client index out of bound." << endl;
		return -1;
	}
	if (ClientSocketFD[ClientIndex] == -1)
	{
		cerr << "ERROR07B Invalid client socket file descriptor number." << endl;
		return -1;
	}

	errorcheck[ClientIndex] = NumOfBytesSent[ClientIndex] = send (ClientSocketFD[ClientIndex], (char *)Data, DataSize, 0);
	if (errorcheck[ClientIndex] == -1)
	{
		cerr << "ERROR007: Server Sending. " << endl;
		return errorcheck[ClientIndex];
	}
	return errorcheck[ClientIndex];
}

// UDP, sendto (data, datasize, IP/name, port);
int CServer::SendTo (void *Data, unsigned int DataSize)
{
	errorcheck[0] = NumOfBytesSent[0] = sendto (ServerSocketFD, (char *)Data, DataSize, 0, (sockaddr *)&TheirAddress, sizeof (TheirAddress));
	if (errorcheck[0] == -1)
	{
		cerr << "ERROR008: Server Sending to. " << endl;
	}
	return errorcheck[0];
}
int CServer::SendTo (void *Data, unsigned int DataSize, char * pTheirIP, int pTheirPort)
{
	struct hostent* TheirIP;		// Server name/IP.
	if ((TheirIP = gethostbyname (pTheirIP)) == NULL)
	{
		cerr << "ERROR009: Getting Their name/IP" << endl;
		return -1;
	}
	// Initializing Their address to send to.
	TheirAddress.sin_family = AF_INET;							// Socket family.
	TheirAddress.sin_addr = *((in_addr *)(*TheirIP).h_addr);		// Their name/IP.
	TheirAddress.sin_port = htons (pTheirPort);			// Their port provided as argument.
	fill ((char*)&(TheirAddress.sin_zero), (char*)&(TheirAddress.sin_zero)+8, '\0');

	errorcheck[0] = NumOfBytesSent[0] = sendto (ServerSocketFD, (char *)Data, DataSize, 0, (sockaddr *)&TheirAddress, sizeof (TheirAddress));
	if (errorcheck[0] == -1)
	{
		cerr << "ERROR010: Server Sending to. " << endl;
	}
	return errorcheck[0];
}
// recvfrom ();
int CServer::RecvFrom ()
{
	socklen_t TheirAddressSize = sizeof (TheirAddress);
	int errorcheck = NumOfBytesReceived[0] = recvfrom (ServerSocketFD, Buffer[0], MAXBUFFERSIZE-1, 0, (sockaddr *)&TheirAddress, &TheirAddressSize);
	if (errorcheck == -1)
	{
		cerr << "ERROR011 Receiveing" << endl;
		return errorcheck;
	}
	Buffer[0][NumOfBytesReceived[0]] = '\0';
	cout << "Server got packet from " << inet_ntoa (TheirAddress.sin_addr) << " on socket " << ServerSocketFD << endl;
	return errorcheck;
}

int CServer::CloseServerSocket ()
{
	int errorcheck = close (ServerSocketFD);
	return errorcheck;
}

int CServer::CloseClientSocket (int ClientIndex)
{
	if (ClientIndex < 0 || ClientIndex >= MAXCLIENTS)
	{
		cerr << "ERROR06A Client index out of bound." << endl;
		return -1;
	}
	if (ClientSocketFD[ClientIndex] == -1)
	{
		cerr << "ERROR06B Invalid client socket file descriptor number." << endl;
		return -1;
	}

	errorcheck[ClientIndex] = close (ClientSocketFD[ClientIndex]);
	ClientSocketFD[ClientIndex] = -1;

	return errorcheck[ClientIndex];
}

int CServer::CloseAllClientSockets()
{
	for (int i=0; i<MAXCLIENTS; i++)
	{
		if (ClientSocketFD[i] != -1)
		{
			errorcheck[i] = close (ClientSocketFD[i]);
			ClientSocketFD[i] = -1;
		}
	}
	return 0;
}

// Additional Functions.
int CServer::DisplayServerInfo ()
{
	cout << "Server Address: " << inet_ntoa (ServerAddress.sin_addr) << endl;
	cout << "Server Port   : " << ntohs (ServerAddress.sin_port) << endl;
	cout << "Server Socket : " << ServerSocketFD << endl;
	return 0;
}

int CServer::DisplayClientInfo (int ClientIndex)
{
	if (ClientIndex < 0 || ClientIndex >= MAXCLIENTS)
	{
		cerr << "ERROR06A Client index out of bound." << endl;
		return -1;
	}
	if (ClientSocketFD[ClientIndex] == -1)
	{
		cerr << "ERROR06B Invalid client socket file descriptor number." << endl;
		return -1;
	}

	cout << "Client Address: " << inet_ntoa (ClientAddress[ClientIndex].sin_addr) << endl;
	cout << "Client Port   : " << ntohs (ClientAddress[ClientIndex].sin_port) << endl;
	cout << "Client Socket : " << ClientSocketFD[ClientIndex] << endl;
	cout << "Computation   : " << ClientsInfo[ClientIndex].ComputationPower << endl;
	return 0;
}

int CServer::DisplayAllClientsInfo ()
{
	for (int i=0; i<MAXCLIENTS; i++)
	{
		if (ClientSocketFD[i] != -1)
		{
			cout << "Client index  : " << endl;
			cout << "Client Address: " << inet_ntoa (ClientAddress[i].sin_addr) << endl;
			cout << "Client Port   : " << ntohs (ClientAddress[i].sin_port) << endl;
			cout << "Client Socket : " << ClientSocketFD[i] << endl;
			cout << "Computation   : " << ClientsInfo[i].ComputationPower << endl;
		}
	}
	return 0;
}
int CServer::DisplayTheirInfo ()
{
	cout << "Their Address: " << inet_ntoa (TheirAddress.sin_addr) << endl;
	cout << "Their Port   : " << ntohs (TheirAddress.sin_port) << endl;
	return 0;
}

int CServer::AcceptClient()
{
	int ClientIndex = Accept ();
	Receive(ClientIndex);
	double* Computation;
	Computation = (double*)GetBuffer(ClientIndex);
	ClientsInfo[ClientIndex].ComputationPower = *Computation;
	DisplayClientInfo (ClientIndex);

	return ClientIndex;
}

int CServer::GetNumOfBytesSent (int ClientIndex)
{
	return NumOfBytesSent[ClientIndex];
}

int CServer::GetNumOfBytesReceived (int ClientIndex)
{
	return NumOfBytesReceived[ClientIndex];
}

char* CServer::GetBuffer (int ClientIndex)
{
	return Buffer[ClientIndex];
}
