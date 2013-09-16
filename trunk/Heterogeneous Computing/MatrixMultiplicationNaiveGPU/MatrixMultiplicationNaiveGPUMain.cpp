#include <MatrixMultiplicationNaiveGPU.hpp>
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	srand(0);
	CMatrixMultiplicationNaiveGPU MatrixMultiplicationNaiveGPUSim(/*Rows=*/1024U, /*Cols=*/1024U);

	// ================== Simulation ================
	MatrixMultiplicationNaiveGPUSim.StartTimer();
	MatrixMultiplicationNaiveGPUSim.CompleteRun(/*Iterations*/5U); // Complete GPU run.
	MatrixMultiplicationNaiveGPUSim.StopTimer();
	cout << "Total time taken = " << MatrixMultiplicationNaiveGPUSim.GetElapsedTime() << " seconds." << endl;

	return 0;
}
