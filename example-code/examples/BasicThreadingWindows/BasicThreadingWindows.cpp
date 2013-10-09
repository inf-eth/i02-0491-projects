#include <process.h>
#include <Windows.h>
#include <iostream>
using namespace std;

void ThreadFunction1 (void *); // Thread 1 prototype.
void ThreadFunction2 (void *); // Thread 2 prototype.

int main()
{
	HANDLE* th;
	th = new HANDLE[3]; // Creating three threads.

	char Alpha[] = "Alpha.";
	char Beta[] = "Beta.";
	char Gamma[] = "Gamma.";

	// Start thread execution.
	th[0] = (HANDLE)_beginthread (ThreadFunction1, 0, (void*)Alpha);
	th[1] = (HANDLE)_beginthread (ThreadFunction2, 0, (void*)Beta);
	th[2] = (HANDLE)_beginthread (ThreadFunction1, 0, (void*)Gamma);

	// Wait for threads to exit before proceeding further.
	for (int i=0; i<3; i++)
		WaitForSingleObject(th[i], INFINITE);

	cout << "All threads have finished execution!" << endl;

	return 0;
}

void ThreadFunction1(void *args)
{
	cout << "My name is thread " << (char*)args << " and I am executing thread function 1." << endl;
	float x = 3.24f;
	float y = 0.34f;
	cout << "Result of " << x << "*" << y << " = " << x*y << endl;
}

void ThreadFunction2(void *args)
{
	cout << "My name is thread " << (char*)args << " and I am executing thread function 2." << endl;
	float x = 1.61f;
	float y = -3.91f;
	cout << "Result of " << x << "+" << y << " = " << x+y << endl;
}
