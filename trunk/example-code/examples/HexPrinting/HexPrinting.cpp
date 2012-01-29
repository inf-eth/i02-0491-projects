#include <iostream>
#include <cstdio>
using namespace std;
int main ()
{
	int x;
	scanf ("%f", &x);
	cout << x << endl;
	unsigned char *p = (unsigned char *)&x;
	for (int i=3; i>=0; i--)
		printf ("%02x ", (*(p+i)));

	return 0;
}
