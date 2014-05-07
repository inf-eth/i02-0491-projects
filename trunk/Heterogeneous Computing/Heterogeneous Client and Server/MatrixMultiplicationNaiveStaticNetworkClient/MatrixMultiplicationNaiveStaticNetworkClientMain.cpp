#if defined __linux__ || defined __CYGWIN__
#include <pthread.h>
#define TRET_TYPE void*
#include <sys/time.h>
#else
#include <process.h>
#define TRET_TYPE void
#endif

#include <Client.h>
#include <MatrixMultiplicationNaiveGPU.hpp>
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

const unsigned int Rows = 1024U;
const unsigned int Cols = 1024U;
const unsigned int Iterations = 10U;

TRET_TYPE DeviceMultiplicationThread (void *);
#if defined __linux__ || defined __CYGWIN__
pthread_mutex_t PlatformMutexLock = PTHREAD_MUTEX_INITIALIZER;
#else
HANDLE PlatformMutexLock = CreateMutex (NULL, FALSE, NULL);
#endif

class DeviceArgument
{
	public:
	unsigned int Rows;
	unsigned int Cols;
	PRECISION* MatA_;
	PRECISION* MatB_;
	PRECISION* MatC_;
	unsigned int ThreadStart;
	unsigned int ThreadEnd;
	unsigned int Platform;
	unsigned int Emulation;
	unsigned int DeviceID;
	unsigned int Iterations;
	char DeviceType[4];
};

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

void InputRandom(PRECISION* Matrix_, unsigned int, unsigned int);
void Display(PRECISION* Matrix_, unsigned int, unsigned int);
void Initialise(PRECISION* Matrix_, unsigned int, unsigned int);
void Multiply(PRECISION*, PRECISION*, PRECISION*, unsigned int, unsigned int);
void Compare(PRECISION*, PRECISION*, unsigned int, unsigned int, unsigned int, unsigned int, PRECISION Multiplier);

int main(int argc, char * argv[])
{
	// Standard error checking. Must provide server name/IP and port to connect.
	if ( argc != 8 )
	{
		cout << "ERROR000: Incorrect input arguments." << endl;
		cout << "Usage: './Client [server name or IP] [server port] [client port] [Computation Rating] [\"ID\"] [Platform] [Emulation]'" << endl;
		cout << "OR" << endl;
		cout << "if using Makefile: 'make runC ARG=\"[server name or IP] [server port] [client port] [Computation Rating] [\"ID\"] [Platform] [Emulation]\"'" << endl;
		cout << "Platform: 1:AMD 2:nVidia." << endl;
		cout << "Emulation: 1:CPU, 2:GPU." << endl;
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

	ClientInfo CInfo;
	CInfo.ComputationPower = atof(argv[4]);
	strncpy(CInfo.ID, argv[5], strlen(argv[5])+1);
	CInfo.Platform = (unsigned int)atoi(argv[6]);
	CInfo.Emulation = (unsigned int)atoi(argv[7]);
	/*
	const double Computation = 1.f/2.4f;
	const char ID[] = "C2D E8400 @3.00 GHz";
	const unsigned int Platform = 1U;
	const unsigned int Emulation = 1U;
	*/
	// Send Client info.
	ClientObj.SendData((void*)&CInfo, sizeof(CInfo));
	/*
	ClientObj.Send ((void *)&Computation, sizeof(double));
	ClientObj.Receive();
	ClientObj.CheckServerACK();
	ClientObj.Send ((void *)ID, sizeof(ID));
	*/
	//ClientObj.Receive();
	//ClientObj.CheckServerACK();

	while (true)
	{
		ClientObj.Receive();
		// Decode instruction.
		if (!strncmp(ClientObj.GetBuffer(), "X00S", 4U))
		{
			SimulationParameters Sim;
			ClientObj.Send((void *)&"ACK", 3U);
			ClientObj.Receive((void*)&Sim, sizeof(Sim));
			ClientObj.Send((void *)&"ACK", 3U);

			srand(0);
			PRECISION* MatrixA_ = new PRECISION[Sim.Rows*Sim.Cols];
			PRECISION* MatrixB_ = new PRECISION[Sim.Rows*Sim.Cols];
			PRECISION* MatrixC_ = new PRECISION[Sim.Rows*Sim.Cols];

			// Matrix A transfer
			ClientObj.ReceiveData((void*)MatrixA_);
			cout << "Matrix A received." << endl;
			// Matrix B transfer
			ClientObj.ReceiveData((void*)MatrixB_);
			cout << "Matrix B received." << endl;
			ClientObj.Send((void *)&"ACK", 3U); // ACK for simulation startup.

			// Initialising Matrix C for simulation.
			Initialise(MatrixC_, Rows, Cols);

			CMatrixMultiplicationNaiveGPU HeterogeneousSim(Sim.Rows, Sim.Cols, MatrixA_, MatrixB_, MatrixC_, Sim.ThreadStart, Sim.ThreadEnd);
			HeterogeneousSim.StartTimer();

			// CL initialisation .
			HeterogeneousSim.InitialiseCL(CInfo.Platform, CInfo.Emulation);
			// Simulation.
			HeterogeneousSim.CompleteRunHeterogeneous(Sim.Iterations); // Complete heterogeneous run.

			HeterogeneousSim.StopTimer();
			double SimTime = HeterogeneousSim.GetElapsedTime();
			ClientObj.SendData((void*)&SimTime, sizeof(double));
			cout << "Total time taken by device " << CInfo.ID << " with client ID " << Sim.DeviceID << " = " << SimTime << " seconds." << endl;

			ClientObj.SendData((void*)(MatrixC_+Sim.ThreadStart*Sim.Cols), (Sim.ThreadEnd-Sim.ThreadStart)*Sim.Cols*sizeof(PRECISION));
			//Display(MatrixC_, Sim.Rows, Sim.Cols);
			/*
			unsigned int DataSize;
			ClientObj.Receive((void*)&DataSize, sizeof(DataSize));
			ClientObj.Send((void *)&"ACK", 3U);

			cout << "Data size: " << DataSize << endl;

			// Transfer Loop.
			unsigned int BytesReceived = 0;
			while (BytesReceived != DataSize)
				BytesReceived += (unsigned int)ClientObj.Receive((void*)((char*)MatrixA_+BytesReceived), DataSize-BytesReceived);

			cout << "Matrix A received." << endl;
			ClientObj.Send((void *)&"ACK", 3U);

			//Display(MatrixA_, Sim.Rows, Sim.Cols);
			*/
			//Compare(MatrixB_, MatrixA_, Sim.Rows, Sim.Cols, 0U, Sim.Rows, (PRECISION)1U);
		}
		if (!strncmp(ClientObj.GetBuffer(), "X00B", 4U))
		{

		}

	}
	
	srand((unsigned int)time(NULL));
	srand(0);
	PRECISION* MatrixA_ = new PRECISION[Rows*Cols];
	PRECISION* MatrixB_ = new PRECISION[Rows*Cols];
	PRECISION* MatrixC_ = new PRECISION[Rows*Cols];
	PRECISION* MatrixCStandard_ = new PRECISION[Rows*Cols];

	InputRandom(MatrixA_, Rows, Cols);
	InputRandom(MatrixB_, Rows, Cols);
	
	Initialise(MatrixC_, Rows, Cols);
	Initialise(MatrixCStandard_, Rows, Cols);

	unsigned int NumberOfDevices = 2U;
	#if defined __linux__ || defined __CYGWIN__
	std::cout << "Running on linux..." << std::endl;
	pthread_t* threads = new pthread_t[NumberOfDevices];
	#else
	std::cout << "Running on windows..." << std::endl;
	HANDLE* th = new HANDLE[NumberOfDevices];
	#endif

	DeviceArgument* Device = new DeviceArgument[NumberOfDevices];

	// ========================= GPU ======================
	Device[0].Rows = Rows;
	Device[0].Cols = Cols;
	Device[0].MatA_ = MatrixA_;
	Device[0].MatB_ = MatrixB_;
	Device[0].MatC_ = MatrixC_;
	Device[0].ThreadStart = 48U;
	Device[0].ThreadEnd = 1024U;
	Device[0].Platform = 2U;
	Device[0].Emulation = 2U;
	Device[0].DeviceID = 0U;
	Device[0].Iterations = Iterations;
	strcpy(Device[0].DeviceType, "GPU");
	// ========================= CPU ======================
	Device[1].Rows = Rows;
	Device[1].Cols = Cols;
	Device[1].MatA_ = MatrixA_;
	Device[1].MatB_ = MatrixB_;
	Device[1].MatC_ = MatrixC_;
	Device[1].ThreadStart = 0U;
	Device[1].ThreadEnd = 48U;
	Device[1].Platform = 1U;
	Device[1].Emulation = 1U;
	Device[1].DeviceID = 1U;
	Device[1].Iterations = Iterations;
	strcpy(Device[1].DeviceType, "CPU");

	__int64 tStart, tEnd;
	cout << "Dispatching workload to devices..." << endl;
	tStart = GetTimeus64();
	for (unsigned int i=0; i<NumberOfDevices; i++)
	{
		#if defined __linux__ || defined __CYGWIN__
		pthread_create (&threads[i], NULL, DeviceMultiplicationThread, (void*)&Device[i]);
		#else
		th[i] = (HANDLE)_beginthread (DeviceMultiplicationThread, 0, (void*)&Device[i]);
		#endif
	}
	for (unsigned int i=0; i<NumberOfDevices; i++)
	{
		#if defined __linux__ || defined __CYGWIN__
		pthread_join (threads[i], NULL);
		#else
		WaitForSingleObject(th[i], INFINITE);
		#endif
	}
	tEnd = GetTimeus64();
	cout << "Time taken by all devices = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	cout << "Performing standard multiplication..." << endl;
	Multiply(MatrixA_, MatrixB_, MatrixCStandard_, Rows, Cols);
	cout << "Comparing results..." << endl;
	Compare(MatrixCStandard_, MatrixC_, Rows, Cols, 0U, 1024U, (PRECISION)Iterations);

	//Display(MatrixC_, Rows, Cols);
	//Display(MatrixCStandard_, Rows, Cols);

	delete[] MatrixA_;
	delete[] MatrixB_;
	delete[] MatrixC_;
	delete[] MatrixCStandard_;

	// Thread cleanup.
	#if defined __linux__ || defined __CYGWIN__
	delete[] threads;
	#else
	delete[] th;
	#endif

	ClientObj.CloseClientSocket ();

	return 0;
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

TRET_TYPE DeviceMultiplicationThread (void *DeviceArgs)
{
	cout << "Rows: " << ((DeviceArgument*)DeviceArgs)->Rows;
	cout << "Cols: " << ((DeviceArgument*)DeviceArgs)->Cols;
	cout << "Thread Start: " << ((DeviceArgument*)DeviceArgs)->ThreadStart;
	cout << "Thread End: " << ((DeviceArgument*)DeviceArgs)->ThreadEnd;
	cout << "Platform: " << ((DeviceArgument*)DeviceArgs)->Platform;
	cout << "Emulation: " << ((DeviceArgument*)DeviceArgs)->Emulation;
	cout << "Iterations: " << ((DeviceArgument*)DeviceArgs)->Iterations;

	CMatrixMultiplicationNaiveGPU HeterogeneousSim(((DeviceArgument*)DeviceArgs)->Rows, ((DeviceArgument*)DeviceArgs)->Cols, ((DeviceArgument*)DeviceArgs)->MatA_, ((DeviceArgument*)DeviceArgs)->MatB_, ((DeviceArgument*)DeviceArgs)->MatC_, ((DeviceArgument*)DeviceArgs)->ThreadStart, ((DeviceArgument*)DeviceArgs)->ThreadEnd);
	HeterogeneousSim.StartTimer();

	#if defined __linux__ || defined __CYGWIN__
	pthread_mutex_lock (&PlatformMutexLock);
	#else
	WaitForSingleObject (PlatformMutexLock, INFINITE);
	#endif
	// CL initialisation must be atomic.
	HeterogeneousSim.InitialiseCL(((DeviceArgument*)DeviceArgs)->Platform, ((DeviceArgument*)DeviceArgs)->Emulation);
	#if defined __linux__ || defined __CYGWIN__
	pthread_mutex_unlock (&PlatformMutexLock);
	#else
	ReleaseMutex (PlatformMutexLock);
	#endif

	HeterogeneousSim.CompleteRunHeterogeneous(Iterations); // Complete heterogeneous run.
	HeterogeneousSim.StopTimer();
	cout << "Total time taken by " << ((DeviceArgument*)DeviceArgs)->DeviceType << " with device ID " << ((DeviceArgument*)DeviceArgs)->DeviceID << " = " << HeterogeneousSim.GetElapsedTime() << " seconds." << endl;

#ifndef WIN32
	return NULL;
#endif
}
