#include "VLI.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
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
	cout << "Primality test for V1 = " << VLIobj1.CheckPrime(true) << endl;
	return 0;
}
