#include "VLI.h"
#include <iostream>
#include <sstream>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
int main (int argc, char **argv)
{
	stringstream ssN, ssM;
	CVLI N, M;
	if (argc == 2)
	{
		ssN << argv[1];
		ssN >> N;
		cout << "N = " << N << endl;
	}
	else if (argc == 3)
	{
		ssN << argv[1];
		ssN >> N;
		ssM << argv[2];
		ssM >> M;
		cout << "N = " << N << endl;
		cout << "M = " << M << endl;
	}
	/*
	CVLI VLIobj1, VLIobj2;
	cout << "Test VLI." << endl;
	cin >> VLIobj1;
	cin >> VLIobj2;
	cout << "VLI1=" << VLIobj1 << endl;
	cout << "VLI2=" << VLIobj2 << endl;
	if (VLIobj1 < VLIobj2)
		cout << "1 < 2" << endl;
	if (VLIobj1 > VLIobj2)
		cout << "1 > 2" << endl;
	cout << "V1 + V2 = " << VLIobj1 + VLIobj2 << endl;
	cout << "V1 - V2 = " << VLIobj1 - VLIobj2 << endl;
	cout << "V1 * V2 = " << VLIobj1 * VLIobj2 << endl;
	cout << "V1 / V2 = " << VLIobj1 / VLIobj2 << endl;
	cout << "V1 % V2 = " << VLIobj1 % VLIobj2 << endl;
	cout << "Primality test for V1 = " << VLIobj1.CheckPrime(true) << endl;*/
	cout << "Primality test for M = " << M.CheckPrime(true) << endl;
	return 0;
}
