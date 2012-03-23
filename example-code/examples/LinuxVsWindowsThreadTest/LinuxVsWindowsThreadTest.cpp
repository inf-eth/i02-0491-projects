#ifdef WIN32
#include <process.h>
#include <Windows.h>
#define TRET_TYPE void
#else
#include <pthread.h>
#define TRET_TYPE void*
#endif

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/time.h>

void PerformanceTest ();
TRET_TYPE thread1f (void *);
TRET_TYPE thread2f (void *);

const unsigned int X  = 1024;
const unsigned int Y  = 1024;
const unsigned int T1  = 500;
const unsigned int T2  = 1000;

double A[X][Y];
double B[X][Y];
double C[X][Y];

struct timeval tv1, tv2;

int main (int argc, char **argv)
{

#ifdef WIN32
	HANDLE th[2];
#else
	pthread_t thread1, thread2;
#endif

	std::cout << "Single thread test: " << X << " * " << Y << " * " << T2 << " multplications." << std::endl;
	gettimeofday(&tv1, NULL);
	PerformanceTest();
	gettimeofday(&tv2, NULL);
	std::cout << "Elapsed time = " << (double)(tv2.tv_sec-tv1.tv_sec) + (double)(tv2.tv_usec-tv1.tv_usec)/(1000000.) << std::endl;

	std::cout << "2x thread test: " << X << " * " << Y << " * " << T1 << " multiplications per thread." << std::endl;
	gettimeofday(&tv1, NULL);
#ifdef WIN32
	th[0] = (HANDLE)_beginthread (thread1f, 0, NULL);
	th[1] = (HANDLE)_beginthread (thread2f, 0, NULL);
	WaitForMultipleObjects (2, th, NULL, INFINITE);
#else
	pthread_create (&thread1, NULL, thread1f, NULL);
	pthread_create (&thread2, NULL, thread2f, NULL);
	pthread_join (thread1, NULL);
	pthread_join (thread2, NULL);
#endif
	gettimeofday(&tv2, NULL);
	std::cout << "Elapsed time = " << (double)(tv2.tv_sec-tv1.tv_sec) + (double)(tv2.tv_usec-tv1.tv_usec)/(1000000.) << std::endl;

	return 0;
}

void PerformanceTest ()
{
	unsigned int i, j, t;
	for (t=0; t<(T1+T2); t++)
		for (i=0; i<X; i++)
			for (j=0; j<Y; j++)
				C[i][j] = A[i][j] * B[i][j];
}

TRET_TYPE thread1f (void *a)
{
	unsigned int i, j, t;
	for (t=0; t<T1; t++)
		for (i=0; i<X; i++)
			for (j=0; j<Y; j++)
				C[i][j] = A[i][j] * B[i][j];
#ifndef WIN32
	return NULL;
#endif
}

TRET_TYPE thread2f (void *a)
{
	unsigned int i, j, t;
	for (t=T1; t<T2; t++)
		for (i=0; i<X; i++)
			for (j=0; j<Y; j++)
				C[i][j] = A[i][j] * B[i][j];
#ifndef WIN32
	return NULL;
#endif
}
