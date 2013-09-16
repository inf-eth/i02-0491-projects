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

#include <Timer.h>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>
using namespace std;

#define DataType double
const int Rows = 1024;
const int Cols = 1024;

TRET_TYPE MultiplicationThread (void *);
TRET_TYPE MultiplicationTransposedThread (void *);

#define Matrix(i,j) Matrix_[Cols*(i)+(j)]
#define MatrixA(i,j) MatrixA_[Cols*(i)+(j)]
#define MatrixB(i,j) MatrixB_[Cols*(i)+(j)]
#define MatrixBt(i,j) MatrixBt_[Cols*(i)+(j)]
#define MatrixC(i,j) MatrixC_[Cols*(i)+(j)]

class Argument
{
	public:
	unsigned int GlobalThreads;
	unsigned int ThreadID;
	int Rows;
	int Cols;
	DataType* MatA_;
	DataType* MatB_;
	DataType* MatC_;
};

void Input(DataType* Matrix_)
{
	for (int i=0; i<Rows; i++)
	{
		cout << "Row " << i << endl;
		for (int j=0; j<Cols; j++)
			cin >> Matrix(i,j);
	}
}

void InputRandom(DataType* Matrix_)
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			Matrix(i,j) = (DataType)rand()/rand();
	}
}

void Display(DataType* Matrix_)
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			cout << setprecision(3) << setw(5) << Matrix(i,j) << "\t";
		cout << endl;
	}
}

void Initialise(DataType* Matrix_)
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			Matrix(i,j) = (DataType)0;
	}
}

void Multiply(DataType* MatrixA_, DataType* MatrixB_, DataType* MatrixC_)
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
		{
			for (int k=0; k<Cols; k++)
				MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
		}
	}
}

void MultiplyTransposed(DataType* MatrixA_, DataType* MatrixBt_, DataType* MatrixC_)
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
		{
			for (int k=0; k<Cols; k++)
				MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixBt(j,k);
		}
	}
}

int main()
{
	__int64 tStart, tEnd;
	int NoOfThreads;
	cout << "Matrix dimensions are: " << Rows << "x" << Cols << endl;
	cout << "Enter number of threads to spawn: ";
	cin >> NoOfThreads;

	#if defined __linux__ || defined __CYGWIN__
	std::cout << "Running on linux..." << std::endl;
	pthread_t* threads = new pthread_t[NoOfThreads];
	#else
	std::cout << "Running on windows..." << std::endl;
	HANDLE* th = new HANDLE[NoOfThreads];
	#endif

	srand((unsigned int)time(NULL));
	DataType* MatrixA_ = new DataType[Rows*Cols];
	DataType* MatrixB_ = new DataType[Rows*Cols];
	DataType* MatrixBt_ = new DataType[Rows*Cols];
	DataType* MatrixC_ = new DataType[Rows*Cols];

	InputRandom(MatrixA_);
	InputRandom(MatrixB_);
	for (int i=0; i<Rows; i++)
		for (int j=0; j<Cols; j++)
			MatrixBt(j,i) = MatrixB(i,j);
	Initialise(MatrixC_);

	Argument* Args = new Argument[NoOfThreads];
	//cout << "Matrix A is :" << endl;
	//Display(MatrixA);
	//cout << "Matrix B is :" << endl;
	//Display(MatrixB);

	// ============ Single threaded implementation ===============
	Initialise(MatrixC_);

	tStart = GetTimeus64();
	Multiply(MatrixA_, MatrixB_, MatrixC_);
	tEnd = GetTimeus64();
	std::cout << "Time taken (single-threaded) = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << std::endl;

	//cout << "Matrix C is :" << endl;
	//Display(MatrixC_);

	// ============ Single threaded transposed implementation ============
	Initialise(MatrixC_);

	tStart = GetTimeus64();
	MultiplyTransposed(MatrixA_, MatrixBt_, MatrixC_);
	tEnd = GetTimeus64();
	std::cout << "Time taken (single-threaded transposed) = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << std::endl;

	//cout << "Matrix C is :" << endl;
	//Display(MatrixC_);

	// ============== Multithreaded ==============
	tStart = GetTimeus64();

	for (int i=0; i<NoOfThreads; i++)
	{
		Args[i].GlobalThreads = NoOfThreads;
		Args[i].Rows = Rows;
		Args[i].Cols = Cols;
		Args[i].MatA_ = MatrixA_;
		Args[i].MatB_ = MatrixB_;
		Args[i].MatC_ = MatrixC_;
		Args[i].ThreadID = i;
		#if defined __linux__ || defined __CYGWIN__
		pthread_create (&threads[i], NULL, MultiplicationThread, (void*)&Args[i]);
		#else
		th[i] = (HANDLE)_beginthread (MultiplicationThread, 0, (void*)&Args[i]);
		#endif
	}
	for (int i=0; i<NoOfThreads; i++)
	{
		#if defined __linux__ || defined __CYGWIN__
		pthread_join (threads[i], NULL);
		#else
		WaitForSingleObject(th[i], INFINITE);
		#endif
	}

	tEnd = GetTimeus64();
	std::cout << "Time taken (multi-threaded) = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << std::endl;

	//cout << "Matrix C is :" << endl;
	//Display(MatrixC_);

	// ============== Multithreaded Transposed ==============
	tStart = GetTimeus64();

		for (int i=0; i<NoOfThreads; i++)
	{
		Args[i].GlobalThreads = NoOfThreads;
		Args[i].Rows = Rows;
		Args[i].Cols = Cols;
		Args[i].MatA_ = MatrixA_;
		Args[i].MatB_ = MatrixBt_;
		Args[i].MatC_ = MatrixC_;
		Args[i].ThreadID = i;
		#if defined __linux__ || defined __CYGWIN__
		pthread_create (&threads[i], NULL, MultiplicationTransposedThread, (void*)&Args[i]);
		#else
		th[i] = (HANDLE)_beginthread (MultiplicationTransposedThread, 0, (void*)&Args[i]);
		#endif
	}
	for (int i=0; i<NoOfThreads; i++)
	{
		#if defined __linux__ || defined __CYGWIN__
		pthread_join (threads[i], NULL);
		#else
		WaitForSingleObject(th[i], INFINITE);
		#endif
	}

	tEnd = GetTimeus64();
	std::cout << "Time taken (multi-threaded transposed) = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << std::endl;

	//cout << "Matrix C is :" << endl;
	//Display(MatrixC_);

	delete[] MatrixA_;
	delete[] MatrixB_;
	delete[] MatrixC_;

	// Thread cleanup.
	#if defined __linux__ || defined __CYGWIN__
	delete[] threads;
	#else
	delete[] th;
	#endif

	return 0;
}

TRET_TYPE MultiplicationThread (void *args)
{
	unsigned int GlobalThreads = ((Argument*)args)->GlobalThreads;
	unsigned int ThreadID = ((Argument*)args)->ThreadID;
	int Rows = ((Argument*)args)->Rows;
	int Cols = ((Argument*)args)->Cols;
	DataType* MatrixA_ = ((Argument*)args)->MatA_;
	DataType* MatrixB_ = ((Argument*)args)->MatB_;
	DataType* MatrixC_ = ((Argument*)args)->MatC_;

	int iStart = ThreadID*Rows/GlobalThreads;
	int iEnd = (ThreadID+1)*Rows/GlobalThreads;

	cout << "This is thread no. " << ThreadID << endl;
	cout << "Range = " << iStart << "-" << iEnd-1 << endl;

	for (int i=iStart; i<iEnd; i++)
	{
		for (int j=0; j<Cols; j++)
		{
			for (int k=0; k<Cols; k++)
				MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixB(k,j);
		}
	}
#ifndef WIN32
	return NULL;
#endif
}

TRET_TYPE MultiplicationTransposedThread (void *args)
{
	unsigned int GlobalThreads = ((Argument*)args)->GlobalThreads;
	unsigned int ThreadID = ((Argument*)args)->ThreadID;
	int Rows = ((Argument*)args)->Rows;
	int Cols = ((Argument*)args)->Cols;
	DataType* MatrixA_ = ((Argument*)args)->MatA_;
	DataType* MatrixBt_ = ((Argument*)args)->MatB_;
	DataType* MatrixC_ = ((Argument*)args)->MatC_;

	int iStart = ThreadID*Rows/GlobalThreads;
	int iEnd = (ThreadID+1)*Rows/GlobalThreads;

	cout << "This is thread no. " << ThreadID << endl;
	cout << "Range = " << iStart << "-" << iEnd-1 << endl;

	for (int i=iStart; i<iEnd; i++)
	{
		for (int j=0; j<Cols; j++)
		{
			for (int k=0; k<Cols; k++)
				MatrixC(i,j) = MatrixC(i,j) + MatrixA(i,k) * MatrixBt(j,k);
		}
	}
#ifndef WIN32
	return NULL;
#endif
}
