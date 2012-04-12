#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

int main ()
{
	int rows = 2;
	int cols = 3;

	// ************************ 2D Array using double pointer ********************
	int **Matrix1;
	Matrix1 = new int*[rows];

	for (int i = 0; i < rows; i++)
		Matrix1[i] = new int[cols];

	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
			cout << i << " " << j << " = " << Matrix1[i][j] << "  ";
		cout << endl;
	}

	// Deallocation takes two steps.
	for (int i = 0; i < rows; i++)
		delete [] Matrix1[i];

	delete [] Matrix1;
	// ***************************************************************************
	
	// *********************** 2D array using single pointer *********************
	int *Matrix2;
	Matrix2 = new int[rows*cols];

	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
			cout << i << " " << j << " = " << Matrix2[i*cols + j] << "  ";
		cout << endl;
	}

	// Single-step deallocation.
	delete [] Matrix2;
	// ***************************************************************************

	// ********************** 3D array using triple pointer **********************
	int ***Matrix3;
	int height = 3;

	// Allocate row pointers
	Matrix3 = new int**[rows];

	// Allocate col pointers
	for (int i=0; i<rows; i++)
		Matrix3[i] = new int*[cols];

	// Allocate height arrays against rows and cols.
	for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			Matrix3[i][j] = new int[height];

	// Input.
	for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			for (int k=0; k<height; k++)
				Matrix3[i][j][k] = rand()%10;

	// Output. Displays 2D slices/arrays height-wise.
	for (int k=0; k<height; k++)
	{
		for (int i=0; i<rows; i++)
		{
			for (int j=0; j<cols; j++)
			{
				cout << "Mat[" << i << "][" << j << "][" << k <<"] = " << Matrix3[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}	

	// Deallocation.
	for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			delete [] Matrix3[i][j];

	for (int i=0; i<rows; i++)
		delete [] Matrix3[i];

	delete [] Matrix3;
	// ***************************************************************************
	return 0;
}
