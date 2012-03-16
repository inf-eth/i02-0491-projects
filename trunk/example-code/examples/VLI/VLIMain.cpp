#define MULTITHREADED true
#define USE_PTHREAD   true

#include "VLI.h"
#include <iostream>
#include <sstream>
#include <ctime>

#if MULTITHREADED
	#if USE_PTHREAD
	#include <pthread.h>
	#else
	#include <omp.h>
	#endif
#endif

using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

// Default check for range 101-17977
int main (int argc, char **argv)
{
	// Timing.
	clock_t Start, End;

	Start = clock();
	if (argc == 2)
	{
		stringstream ssN;
		CVLI N;

		ssN << argv[1];
		ssN >> N;
		cout << "N = " << N << endl;
		cout << "Primality test for N = " << N.CheckPrime(true) << endl;
	}
	else if (argc == 3)
	{
		stringstream ssN, ssM;
		CVLI N, M;
		vector<CVLI> PrimeList;

		ssN << argv[1];
		ssN >> N;
		ssM << argv[2];
		ssM >> M;
		cout << "N = " << N << endl;
		cout << "M = " << M << endl;

		// CVLI variables for iteration. Assuming N is larger than 3.
		bool odd = false;
		CVLI i;
		CVLI one;
		CVLI six;
		CVLI Check;
		char oneS[] = "1";
		char sixS[] = "6";
		ssN.clear();
		ssN << oneS;
		ssN >> one;
		ssN.clear();
		ssN << sixS;
		ssN >> six;
		i = ++((N-one)/six);
		Check = (six*i)-one;
		
		if (Check <= N)
		{
			Check = (six*i++)+one;
			odd = !odd;
		}

		cout << "one = " << one << endl;
		cout << "six = " << six << endl;
		cout << "i = " << i << endl;
		cout << "Check = " << Check << endl;
		
		#if !MULTITHREADED
		while (Check < M)
		{
			if (Check.CheckPrime() == true)
			{
				cout << Check << " is prime." << endl;
				//PrimeList.push_back (Check);
			}
			if (odd == false)
			{
				Check = (six*(i++))+one;
				odd = !odd;
			}
			else
			{
				Check = (six*i)-one;
				odd = !odd;
			}
		}
		#else
		CVLI Oddi = i;
		CVLI Eveni = i;
		CVLI OddCheck = odd == false ? Check : (six*i)-one;
		CVLI EvenCheck = odd == false ? (six*i)+one : Check;

		int tid = 0;
		int np = 1;
		#pragma omp parallel default(shared) private(tid, np)
		{
			#if defined (_OPENMP)
			tid = omp_get_thread_num();
			np = omp_get_num_threads();
			#endif
			//std::cout << "Hello from thread " << tid << " out of " << np << std::endl;
			// Odd section.
			if (tid == 0)
			{
				while (OddCheck < M)
				{
					if (OddCheck.CheckPrime() == true)
						cout << OddCheck << " is prime." << endl;
					Oddi++;
					OddCheck = (six*Oddi)-one;
				}
			}
			// Even section.
			else if (tid == 1)
			{
				while (EvenCheck < M)
				{
					if (EvenCheck.CheckPrime() == true)
						cout << EvenCheck << " is prime." << endl;
					Eveni++;
					EvenCheck = (six*Eveni)+one;
				}
			}
		}
		#endif
	}

	End = clock();
	cout << "Time taken = " << (double)(End - Start)/CLOCKS_PER_SEC << " seconds." << endl;

	return 0;
}