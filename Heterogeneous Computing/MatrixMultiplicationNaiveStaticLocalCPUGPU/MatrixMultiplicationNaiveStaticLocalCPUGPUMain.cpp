#include <MatrixMultiplicationNaiveGPU.hpp>
#include <iomanip>
#include <iostream>
using namespace std;

const unsigned int Rows = 1024U;
const unsigned int Cols = 1024U;

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

	// ========================= GPU ======================
	CMatrixMultiplicationNaiveGPU HeterogeneousSimGPU(Rows, Cols, MatrixA_, MatrixB_, MatrixC_, 48U, 1024U);
	HeterogeneousSimGPU.StartTimer();
	HeterogeneousSimGPU.CompleteRunHeterogeneous(2U, 2U); // Complete GPU run.
	HeterogeneousSimGPU.StopTimer();
	cout << "Total time taken = " << HeterogeneousSimGPU.GetElapsedTime() << " seconds." << endl;

	// ========================= CPU ======================
	CMatrixMultiplicationNaiveGPU HeterogeneousSimCPU(Rows, Cols, MatrixA_, MatrixB_, MatrixC_, 0U, 48U);
	HeterogeneousSimCPU.StartTimer();
	HeterogeneousSimCPU.CompleteRunHeterogeneous(1U, 1U); // Complete GPU run.
	HeterogeneousSimCPU.StopTimer();
	cout << "Total time taken = " << HeterogeneousSimCPU.GetElapsedTime() << " seconds." << endl;

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
