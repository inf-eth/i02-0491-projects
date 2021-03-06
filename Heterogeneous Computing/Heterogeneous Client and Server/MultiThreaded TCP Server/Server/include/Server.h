// Server Interface. Server can be TCP or UDP.
#if defined __linux__ || defined __CYGWIN__
#include <netdb.h>			// gethostbyname (), bind(), listen(), connect(), accept(), send(), receive()
#include <arpa/inet.h>		// inet_ntoa(), bind(), listen(), connect(), accept(), send(), receive()
#include <unistd.h>			// close()
#define close close
#else
#include <WinSock2.h>
#define close closesocket
#define socklen_t int
#endif

#define MAXCLIENTS		64
#define MAXBUFFERSIZE	512
#define TCPSOCKET		0
#define UDPSOCKET		1
#define DEFAULTSERVERPORT		6000

// Server Codes.
// X00A: Get ready to receive matrix A with this size.
// X00B: Get ready to receive matrix B with this size.
// X00S: Get ready to receive simulation information.
// X00X: Start simulation.
// X00C: Send simulation data back.

class ClientInfo
{
public:
	double ComputationPower;
	char ID[64];
	unsigned int Platform;
	unsigned int Emulation;
};
class CServer
{
private:
	int Type;		// TCP or UDP.
	// Addresses.
	struct sockaddr_in ServerAddress;	// Server's Address structure.
	struct sockaddr_in ClientAddress[64];	// Client's Address structure.
	struct sockaddr_in TheirAddress;	// Default address to sendto() and recvfrom() UDP packets.

	// File Descriptors.
	int ServerSocketFD;
	int ClientSocketFD[64];

	// Server's Buffer.
	char Buffer[MAXCLIENTS][MAXBUFFERSIZE];
	int  NumOfBytesReceived[MAXCLIENTS];
	int  NumOfBytesSent[MAXCLIENTS];

	// Miscellaneous Variables.
	int ServerPort;
	int errorcheck[MAXCLIENTS];
	int Connections;

public:
	ClientInfo ClientsInfo[MAXCLIENTS];
	CServer (int = TCPSOCKET, int = DEFAULTSERVERPORT);
	// Socket function wrappers.
	int CreateSocket (int = TCPSOCKET);			// 0 = TCP, 1 = UDP; default is to create TCP socket.
	int SetSocketOptions ();					// Set socket options to reuse address.
	int InitializeAddress (int = DEFAULTSERVERPORT);	// Default Server port is 5000.

	int Bind ();								// Bind Server socket with address.
	int Listen ();								// Listen for incoming connections; for TCP Server.
	int Accept ();								// Accept incoming connections.

	// TCP send() and recv()
	int Send (void *, int, int=0);
	int Receive (int=0);
	int Receive(void*, int, int=0);
	int SendData(void *, int, int=0);
	int ReceiveData(void *, int=0);

	// UDP, sendto (data, datasize, IP/name, port);
	int SendTo (void *, int);
	int SendTo (void *, int, char *, int);
	// recvfrom ();
	int RecvFrom ();
	
	int CloseServerSocket ();
	int CloseClientSocket (int=0);
	int CloseAllClientSockets();

	// Additional functions.
	int GetType () { return Type; }
	int GetPort () { return ServerPort; }
	int GetErrorCheck (int ClientIndex=0) { return errorcheck[ClientIndex]; }
	int GetConnections() {return Connections;}

	int DisplayServerInfo ();
	int DisplayClientInfo (int=0);
	int DisplayAllClientsInfo ();
	int DisplayTheirInfo ();

	// Client communication.
	int AcceptClient();

	char* GetBuffer (int=0);
	int GetNumOfBytesSent (int=0);
	int GetNumOfBytesReceived (int=0);
};
