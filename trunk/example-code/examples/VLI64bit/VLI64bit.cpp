#include <iostream>
#include <sstream>
#include <ctime>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

#define VLI64 unsigned long long

bool CheckPrime (const VLI64&);
int DigitCount (const VLI64&);
VLI64 Power10 (int n);
VLI64 Sqrt (const VLI64&, int = 5);

// Default check for range 101-17977
int main (int argc, char **argv)
{
	clock_t Start, End;
	Start = clock();
	
	VLI64 x = 1171432692373ULL;
	cout << "x = " << x << endl;
	cout << "Primality test for x: " << CheckPrime (x) << endl;

	End = clock();
	cout << "Time taken = " << (double)(End - Start)/CLOCKS_PER_SEC << " seconds." << endl;

	return 0;
}

bool CheckPrime (const VLI64& VLICheck)
{
	// Base cases.
	if (VLICheck == 1ULL || VLICheck == 2ULL || VLICheck == 3ULL || VLICheck == 5ULL || VLICheck == 7ULL || VLICheck == 11ULL)
		return true;

	// Division by 2 and 3.
	if (VLICheck % 2ULL == 0ULL || VLICheck % 3ULL == 0ULL)
		return false;

	VLI64 i = 1ULL;
	bool odd = false;
	VLI64 Current = 6ULL*i-1ULL;

	int n, D;
	VLI64 UpperBound;

	D = DigitCount (VLICheck);

	// Determine uppperbound.
	if (DigitCount(VLICheck) % 2 == 1)
	{
		n = (D-1)/2;
		UpperBound = 4ULL * Power10 (n);
	}
	else
	{
		n = (D-2)/2;
		UpperBound = 12ULL * Power10 (n);
	}

	while (Current < UpperBound)
	{
		if (VLICheck % Current == 0ULL)
			return false;

		odd == false ? Current = (6ULL*i++)+1ULL : Current = (6ULL*i)-1ULL;
		odd = !odd;
	}
	return true;
}

int DigitCount (const VLI64& VLI)
{
	if (VLI == 0ULL)
		return 1;
	
	int digits = 0;
	VLI64 temp = VLI;
	while (temp != 0)
	{
		temp = temp / 10ULL;
		digits++;
	}
	return digits;
}

VLI64 Power10 (int n)
{
	VLI64 pow = 1ULL;
	for (int i=0; i<n; i++)
		pow = pow * 10ULL;
	return pow;
}

VLI64 Sqrt (const VLI64& VLI, int Iterations)
{
	int n;
	int D = DigitCount (VLI);
	VLI64 x;
	
	// Determine guess.
	if (DigitCount(VLI) % 2 == 1)
	{
		n = (D-1)/2;
		x = 2ULL * Power10 (n);
	}
	else
	{
		n = (D-2)/2;
		x = 6ULL * Power10 (n);
	}
	for (int i=0; i<Iterations; i++)
	{
		x = (x+(VLI/x))/2ULL;
	}
	return x;
}