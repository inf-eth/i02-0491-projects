#include <iostream>
using std::cout;
using std::endl;

int main ()
{
	int rows = 3;
	int cols = 4;

	// 2D Array using double pointer.
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

	// 2D array using single pointer.
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

	return 0;
}
