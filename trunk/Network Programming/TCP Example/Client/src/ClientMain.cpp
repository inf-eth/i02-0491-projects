#include <Client.h>
#include <cstdlib>			// exit(), atoi()
#include <cstring>			// strlen()
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main()
{
	// Create Client object.
	Client ClientObj;

	// Create Client Socket.
	ClientObj.CreateSocket(TCPSOCKET);
	ClientObj.SetSocketOptions();

	// Initialise and bind Client address.
	ClientObj.InitialiseAddress(6001);
	ClientObj.Bind();

	char ServerName[24];
	int ServerPort;
	cout << "Enter Server name or IP: "; // Use localhost or 127.0.0.1 for local server.
	cin.getline(ServerName, 24);
	cout << "Enter Server port: ";
	cin >> ServerPort;

	// Connect to Server. Server name/IP and port are provided as arguments.
	ClientObj.Connect(ServerName, ServerPort);

	// Send and receive.
	char ClientMessage[] = "Hello from client.";
	ClientObj.Send((void*)ClientMessage, (unsigned int)strlen(ClientMessage));

	ClientObj.Receive();
	cout << "Server says: " << ClientObj.GetBuffer() << endl;

	// Receive a number.
	int x;
	ClientObj.Receive((void*)&x, sizeof(x));
	cout << "Received number: " << x << endl;

	// Receive an array.
	int Data[5];
	ClientObj.Receive((void*)&Data, sizeof(Data));
	cout << "Received array: ";
	for (int i=0; i<5; i++)
		cout << Data[i] << " ";
	cout << endl;

	// Send a structure.
	class Complex
	{
	public:
		float real;
		float img;
	};
	Complex C;
	ClientObj.Receive((void*)&C, sizeof(C));
	cout << "Received complex: " << C.real << "+j(" << C.img << ")" << endl;

	// Closing connection.
	ClientObj.CloseClientSocket();

	return 0;
}
