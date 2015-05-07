#include <Server.h>
#include <cstring>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main()
{
	// Create Server object.
	Server ServerObj;

	// Create Server socket and set socket options.
	ServerObj.CreateSocket(TCPSOCKET);
	ServerObj.SetSocketOptions();

	// Initialise Server address struct and bind it with Server's socket.
	ServerObj.InitialiseAddress(6000);
	ServerObj.Bind();

	// Listen for incoming connections.
	ServerObj.Listen();
	ServerObj.DisplayServerInfo();

	// Accept any incoming connections.
	ServerObj.Accept();
	ServerObj.DisplayClientInfo();

	// Send and receive message.
	ServerObj.Receive();
	cout << "No. of bytes received: " << ServerObj.GetNumOfBytesReceived() << endl;
	cout << "Client says: " << ServerObj.GetBuffer() << endl;

	char message[] = "Hello from server.";
	ServerObj.Send((void*)message, (unsigned int)strlen(message));

	// Send a number.
	int x = 25;
	ServerObj.Send((void*)&x, sizeof(x));

	// Send an array.
	int Data[5] = {12, 33, -1, 4, 21};
	ServerObj.Send((void*)&Data, sizeof(Data));

	// Send a structure.
	class Complex
	{
	public:
		float real;
		float img;
	};
	Complex C;
	C.real = 3.1;
	C.img = -1.4;
	ServerObj.Send((void*)&C, sizeof(C));

	// Close sockets.
	ServerObj.CloseClientSocket();
	ServerObj.CloseServerSocket();

	return 0;
}
