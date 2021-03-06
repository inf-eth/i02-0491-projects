#if defined __linux__ || defined __CYGWIN__
#include <pthread.h>
#define TRET_TYPE void*
#include <sys/time.h>
#else
#include <process.h>
#define TRET_TYPE void
#endif

#define PRECISION double

#include <Server.h>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <Timer.h>
using namespace std;

const unsigned int Rows = 1024U;
const unsigned int Cols = 1024U;
const unsigned int Iterations = 10U;

class SimulationParameters
{
	public:
	unsigned int Rows;
	unsigned int Cols;
	unsigned int ThreadStart;
	unsigned int ThreadEnd;
	unsigned int DeviceID;
	unsigned int Iterations;
};

class WorkerArgs
{
	public:
	CServer* ServerObj;
	SimulationParameters* Sim;
	PRECISION* MatrixA_;
	PRECISION* MatrixB_;
	PRECISION* MatrixC_;
	int Index;
};

TRET_TYPE AcceptorThread (void *);
TRET_TYPE ClientHandlerThread (void *);
TRET_TYPE ClientWorkerThread (void *);

#if defined __linux__ || defined __CYGWIN__
pthread_mutex_t PlatformMutexLock = PTHREAD_MUTEX_INITIALIZER;
#else
HANDLE PlatformMutexLock = CreateMutex (NULL, FALSE, NULL);
#endif

void InputRandom(PRECISION* Matrix_, unsigned int, unsigned int);
void Display(PRECISION* Matrix_, unsigned int, unsigned int);
void Initialise(PRECISION* Matrix_, unsigned int, unsigned int);
void Multiply(PRECISION*, PRECISION*, PRECISION*, unsigned int, unsigned int);
void Compare(PRECISION*, PRECISION*, unsigned int, unsigned int, unsigned int, unsigned int, PRECISION Multiplier);

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
		cout << "5. Start computation on client (by index)." << endl;
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
				// Making list of clients connected.
				int ClientsConnected = ServerObj.GetConnections();
				int QuantisedWorkItems = Rows/4;
				double TotalComputingPower = 0.;
				vector<int> Workload(ClientsConnected);
				vector<ClientInfo> ClientInfoList(ClientsConnected);
				vector<SimulationParameters> SimData(ClientsConnected);

				for (int i=0; i<ClientsConnected; i++)
				{
					if (ServerObj.ClientsInfo[i].ComputationPower > 0.)
					{
						ClientInfoList[i] = ServerObj.ClientsInfo[i];
						TotalComputingPower = TotalComputingPower + ClientInfoList[i].ComputationPower;
					}
				}

				unsigned int ThreadStart = 0U;
				for (int i=0; i<ClientsConnected; i++)
				{
					if (i==ClientsConnected-1)
						Workload[i] = 4*(int)((QuantisedWorkItems*(ClientInfoList[i].ComputationPower/TotalComputingPower))+1);
					else
						Workload[i] = 4*(int)(QuantisedWorkItems*(ClientInfoList[i].ComputationPower/TotalComputingPower));

					SimData[i].Rows = Rows;
					SimData[i].Cols = Cols;
					SimData[i].ThreadStart = ThreadStart;
					SimData[i].ThreadEnd = ThreadStart+(unsigned int)Workload[i];
					SimData[i].DeviceID = i;
					SimData[i].Iterations = Iterations;

					ThreadStart = ThreadStart+(unsigned int)Workload[i];
				}

				// Simulation information.
				cout << "Total connected clients: " << ClientsConnected << endl;
				cout << "Total Computing power:   ";
				for (int i=0; i<ClientsConnected; i++)
					cout << ClientInfoList[i].ComputationPower << " ";
				cout << "= " << TotalComputingPower << endl;
				cout << "Total work items:        " << Rows << endl;
				cout << "Client workload share:   ";

				for (int i=0; i<ClientsConnected; i++)
				{
					cout << "Client Index: " << i << endl;
					cout << "Workload:     " << Workload[i] << endl;
					cout << "Thread start: " << SimData[i].ThreadStart << endl;
					cout << "Thread end:   " << SimData[i].ThreadEnd << endl;
				}

				srand(0);
				PRECISION* MatrixA_ = new PRECISION[Rows*Cols];
				PRECISION* MatrixB_ = new PRECISION[Rows*Cols];
				PRECISION* MatrixC_ = new PRECISION[Rows*Cols];

				InputRandom(MatrixA_, Rows, Cols);
				InputRandom(MatrixB_, Rows, Cols);

				// Threads.
				#if defined __linux__ || defined __CYGWIN__
				pthread_t* Clientth = new pthread_t[ClientsConnected];
				#else
				HANDLE* Clientth = new HANDLE[ClientsConnected];
				#endif

				__int64 tStart, tEnd;
				cout << "Dispatching workload to devices..." << endl;
				tStart = GetTimeus64();

				WorkerArgs* Args = new WorkerArgs[ClientsConnected];
				for (int i=0; i<ClientsConnected; i++)
				{
					Args[i].ServerObj = &ServerObj;
					Args[i].Sim = &SimData[i];
					Args[i].MatrixA_ = MatrixA_;
					Args[i].MatrixB_ = MatrixB_;
					Args[i].MatrixC_ = MatrixC_;
					Args[i].Index = i;

					#if defined __linux__ || defined __CYGWIN__
					pthread_create (&Clientth[i], NULL, ClientWorkerThread, (void*)&Args[i]);
					#else
					Clientth[i] = (HANDLE)_beginthread (ClientWorkerThread, 0, (void*)&Args[i]);
					#endif
				}
				for (unsigned int i=0; i<(unsigned int)ClientsConnected; i++)
				{
					#if defined __linux__ || defined __CYGWIN__
					pthread_join (Clientth[i], NULL);
					#else
					WaitForSingleObject(Clientth[i], INFINITE);
					#endif
				}

				tEnd = GetTimeus64();
				cout << "Matrix C received." << endl;
				cout << "* Time taken by all devices = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
				cout << "Verifiying results..." << endl;
				PRECISION* MatrixCStandard_ = new PRECISION[Rows*Cols];
				Initialise(MatrixCStandard_, Rows, Cols);
				cout << "Performing standard multiplication..." << endl;
				Multiply(MatrixA_, MatrixB_, MatrixCStandard_, Rows, Cols);
				cout << "Comparing results..." << endl;
				Compare(MatrixCStandard_, MatrixC_, Rows, Cols, 0U, Rows, (PRECISION)Iterations);

				delete[] Args;
				delete[] Clientth;
				delete[] MatrixA_;
				delete[] MatrixB_;
				delete[] MatrixC_;
				delete[] MatrixCStandard_;

				break;
			}
			case '5':
			{
				int Index;
				cout << "Enter index: ";
				cin >> Index;
				ServerObj.DisplayClientInfo(Index);

				SimulationParameters Sim;
				Sim.Rows = Rows;
				Sim.Cols = Cols;
				Sim.ThreadStart = 0U;
				Sim.ThreadEnd = Rows;
				Sim.DeviceID = Index;
				Sim.Iterations = Iterations;

				ServerObj.Send((void*)&"X00S", 4U, Index);
				ServerObj.Receive(Index);
				ServerObj.Send((void*)&Sim, sizeof(Sim), Index);
				ServerObj.Receive(Index);

				/*
				cout << "Sending A info" << endl;

				ServerObj.Send((void*)&"X00A", 4U, Index);
				ServerObj.Receive(Index);
				*/
				srand(0);
				PRECISION* MatrixA_ = new PRECISION[Rows*Cols];
				PRECISION* MatrixB_ = new PRECISION[Rows*Cols];
				PRECISION* MatrixC_ = new PRECISION[Rows*Cols];

				InputRandom(MatrixA_, Rows, Cols);
				InputRandom(MatrixB_, Rows, Cols);

				//Display(MatrixA_, Sim.Rows, Sim.Cols);
				ServerObj.SendData((void*)MatrixA_, Rows*Cols*sizeof(PRECISION), Index);
				ServerObj.SendData((void*)MatrixB_, Rows*Cols*sizeof(PRECISION), Index);
				ServerObj.Receive(Index);
				cout << "Simulation started..." << endl;
				double SimTime;
				ServerObj.ReceiveData((void*)&SimTime, Index);
				cout << "Total time taken by device " << ServerObj.ClientsInfo[Index].ID << " with client ID " << Sim.DeviceID << " = " << SimTime << " seconds." << endl;

				// Results verification.
				ServerObj.ReceiveData((void*)(MatrixC_+Sim.ThreadStart*Sim.Cols), Index);

				cout << "Matrix C received." << endl;
				cout << "Verifiying results..." << endl;
				PRECISION* MatrixCStandard_ = new PRECISION[Sim.Rows*Sim.Cols];
				Initialise(MatrixCStandard_, Rows, Cols);
				cout << "Performing standard multiplication..." << endl;
				Multiply(MatrixA_, MatrixB_, MatrixCStandard_, Rows, Cols);
				cout << "Comparing results..." << endl;
				Compare(MatrixCStandard_, MatrixC_, Sim.Rows, Sim.Cols, Sim.ThreadStart, Sim.ThreadEnd, (PRECISION)Sim.Iterations);

				//Display(MatrixC_, Sim.Rows, Sim.Cols);
				//Display(MatrixCStandard_, Sim.Rows, Sim.Cols);
				/*
				unsigned int DataSize = Sim.Rows*Sim.Cols*sizeof(PRECISION);
				ServerObj.Send((void*)&DataSize, sizeof(DataSize), Index);
				ServerObj.Receive(Index);

				cout << "Data size: " << DataSize << endl;

				srand(0);
				PRECISION* MatrixA_ = new PRECISION[Rows*Cols];
				InputRandom(MatrixA_, Rows, Cols);
				//Display(MatrixA_, Sim.Rows, Sim.Cols);

				cout << "Sending A" << endl;

				unsigned int BytesSent = 0;
				while (BytesSent != DataSize)
					BytesSent += (unsigned int)ServerObj.Send((void*)((char*)MatrixA_+BytesSent), DataSize-BytesSent, Index);

				cout << "Matrix A Sent." << endl;
				ServerObj.Receive(Index);
				cout << "Client ACK received." << endl;
				*/
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

TRET_TYPE ClientWorkerThread (void *arg)
{
	CServer* ServerObj = ((WorkerArgs*)arg)->ServerObj;
	SimulationParameters* Sim = ((WorkerArgs*)arg)->Sim;
	PRECISION* MatrixA_ = ((WorkerArgs*)arg)->MatrixA_;
	PRECISION* MatrixB_ = ((WorkerArgs*)arg)->MatrixB_;
	PRECISION* MatrixC_ = ((WorkerArgs*)arg)->MatrixC_;
	int Index = ((WorkerArgs*)arg)->Index;

	ServerObj->Send((void*)&"X00S", 4U, Index);
	ServerObj->Receive(Index);
	ServerObj->Send((void*)&(*Sim), sizeof(*Sim), Index);
	ServerObj->Receive(Index);

	ServerObj->SendData((void*)MatrixA_, Rows*Cols*sizeof(PRECISION), Index);
	ServerObj->SendData((void*)MatrixB_, Rows*Cols*sizeof(PRECISION), Index);
	ServerObj->Receive(Index);
	cout << "Simulation started..." << endl;
	double SimTime;
	ServerObj->ReceiveData((void*)&SimTime, Index);
	cout << "Total time taken by device " << ServerObj->ClientsInfo[Index].ID << " with client ID " << Sim->DeviceID << " = " << SimTime << " seconds." << endl;

	// Results verification.
	ServerObj->ReceiveData((void*)(MatrixC_+Sim->ThreadStart*Sim->Cols), Index);

#ifndef WIN32
	return NULL;
#endif
}

#define Matrix(i,j) Matrix_[Cols*(i)+(j)]
#define MatrixA(i,j) MatrixA_[Cols*(i)+(j)]
#define MatrixB(i,j) MatrixB_[Cols*(i)+(j)]
#define MatrixC(i,j) MatrixC_[Cols*(i)+(j)]

void InputRandom(PRECISION* Matrix_, unsigned int Rows, unsigned int Cols)
{
	for (unsigned int i=0; i<Rows; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
			Matrix(i,j) = (PRECISION)rand()/rand();
	}
}

void Display(PRECISION* Matrix_, unsigned int Rows, unsigned int Cols)
{
	for (unsigned int i=0; i<Rows; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
			cout << setprecision(3) << setw(5) << Matrix(i,j) << "\t";
		cout << endl;
	}
}

void Initialise(PRECISION* Matrix_, unsigned int Rows, unsigned int Cols)
{
	for (unsigned int i=0; i<Rows; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
			Matrix(i,j) = (PRECISION)0;
	}
}

void Multiply(PRECISION* MatrixA_, PRECISION* MatrixB_, PRECISION* MatrixC_, unsigned int Rows, unsigned int Cols)
{
	for (unsigned int i=0; i<Rows; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
		{
			for (unsigned int k=0; k<Cols; k++)
				MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
		}
	}
}

void Compare(PRECISION* MatrixA_, PRECISION* MatrixB_, unsigned int Rows, unsigned int Cols, unsigned int ThreadStart, unsigned int ThreadEnd, PRECISION Multiplier)
{
	for (unsigned int i=ThreadStart; i<ThreadEnd; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
		{
			const PRECISION Difference = MatrixA(i,j) - MatrixB(i,j)/Multiplier;
			if (Difference < (PRECISION)-0.05 || Difference > (PRECISION)0.05)
			{
				cout << "Results don't match!" << endl;
				cout << "i = " << i << ", j = " << j << endl;
				return;
			}
		}
	}
	cout << "Results verified!" << endl;
}
