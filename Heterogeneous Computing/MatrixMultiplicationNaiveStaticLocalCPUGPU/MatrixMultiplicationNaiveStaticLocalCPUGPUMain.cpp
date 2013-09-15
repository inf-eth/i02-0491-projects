#if defined __linux__ || defined __CYGWIN__
#include <pthread.h>
#define TRET_TYPE void*
#include <sys/time.h>
#else
#include <process.h>
#include <Windows.h>
#include <Timer.h>
#define TRET_TYPE void
#endif

#include <MatrixMultiplicationNaiveGPU.hpp>
#include <iomanip>
#include <iostream>
using namespace std;

const unsigned int Rows = 1024U;
const unsigned int Cols = 1024U;

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
	char DeviceType[4];
};

void InputRandom(PRECISION* Matrix_, unsigned int, unsigned int);
void Display(PRECISION* Matrix_, unsigned int, unsigned int);
void Initialise(PRECISION* Matrix_, unsigned int, unsigned int);
void Multiply(PRECISION*, PRECISION*, PRECISION*, unsigned int, unsigned int);
void Compare(PRECISION*, PRECISION*, unsigned int, unsigned int, unsigned int, unsigned int);

int main(int argc, char * argv[])
{
	//CMatrixMultiplicationNaiveGPU MatrixMultiplicationNaiveGPUSim(/*Rows=*/Rows, /*Cols=*/Cols);
	/*
	// ================== Simulation ================
	MatrixMultiplicationNaiveGPUSim.StartTimer();
	MatrixMultiplicationNaiveGPUSim.CompleteRun(); // Complete GPU run.
	MatrixMultiplicationNaiveGPUSim.StopTimer();
	cout << "Total time taken = " << MatrixMultiplicationNaiveGPUSim.GetElapsedTime() << " seconds." << endl;
	*/
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
	#if defined __linux__ || defined __CYGWIN__
	for (int i=0; i<NoOfThreads; i++)
		pthread_join (threads[i], NULL);
	#else
	WaitForMultipleObjects (NumberOfDevices, th, NULL, INFINITE);
	#endif
	tEnd = GetTimeus64();
	cout << "Time taken by all devices = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	cout << "Performing standard multiplication..." << endl;
	Multiply(MatrixA_, MatrixB_, MatrixCStandard_, Rows, Cols);
	cout << "Comparing results..." << endl;
	Compare(MatrixCStandard_, MatrixC_, Rows, Cols, 0U, 1024U);

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

void Compare(PRECISION* MatrixA_, PRECISION* MatrixB_, unsigned int Rows, unsigned int Cols, unsigned int ThreadStart, unsigned int ThreadEnd)
{
	for (unsigned int i=ThreadStart; i<ThreadEnd; i++)
	{
		for (unsigned int j=0; j<Cols; j++)
		{
			const PRECISION Difference = MatrixA(i,j) - MatrixB(i,j);
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

	HeterogeneousSim.CompleteRunHeterogeneous(); // Complete heterogeneous run.
	HeterogeneousSim.StopTimer();
	cout << "Total time taken by " << ((DeviceArgument*)DeviceArgs)->DeviceType << " with device ID " << ((DeviceArgument*)DeviceArgs)->DeviceID << " = " << HeterogeneousSim.GetElapsedTime() << " seconds." << endl;

#ifndef WIN32
	return NULL;
#endif
}
