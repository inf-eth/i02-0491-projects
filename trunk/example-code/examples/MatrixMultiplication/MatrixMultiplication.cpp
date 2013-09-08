#include <Timer.h>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>
using namespace std;

#define DataType double
const int Rows = 512;
const int Cols = 512;

void Input(DataType Matrix[][Cols])
{
	for (int i=0; i<Rows; i++)
	{
		cout << "Row " << i << endl;
		for (int j=0; j<Cols; j++)
			cin >> Matrix[i][j];
	}
}

void InputRandom(DataType Matrix[][Cols])
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			Matrix[i][j] = (DataType)rand()/rand();
	}
}

void Display(DataType Matrix[][Cols])
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			cout << setprecision(3) << setw(5) << Matrix[i][j] << "\t";
		cout << endl;
	}
}

void Initialise(DataType Matrix[][Cols])
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
			Matrix[i][j] = (DataType)0;
	}
}

void Multiply(DataType MatrixA[][Cols], DataType MatrixB[][Cols], DataType MatrixC[][Cols])
{
	for (int i=0; i<Rows; i++)
	{
		for (int j=0; j<Cols; j++)
		{
			for (int k=0; k<Cols; k++)
				MatrixC[i][j] = MatrixC[i][j] + MatrixA[i][k] * MatrixB[k][j];
		}
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	DataType MatrixA[Rows][Cols];
	DataType MatrixB[Rows][Cols];
	DataType MatrixC[Rows][Cols];

	cout << "here " << endl;

	InputRandom(MatrixA);
	InputRandom(MatrixB);
	Initialise(MatrixC);

	//cout << "Matrix A is :" << endl;
	//Display(MatrixA);
	//cout << "Matrix B is :" << endl;
	//Display(MatrixB);

	__int64 tStart, tEnd;
	tStart = GetTimeus64();

	Multiply(MatrixA, MatrixB, MatrixC);

	tEnd = GetTimeus64();
	std::cout << "Time taken = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << std::endl;

	//cout << "Matrix C is :" << endl;
	//Display(MatrixC);

	return 0;
}
