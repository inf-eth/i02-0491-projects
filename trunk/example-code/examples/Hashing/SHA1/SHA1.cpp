// Reference: http://stackoverflow.com/questions/918676/generate-sha-hash-in-openssl

#include <iomanip>
#include <iostream>
#include <cstring>
#include <openssl/sha.h>

using std::cout;
using std::hex;
using std::setfill;
using std::setw;
using std::endl;

int main()
{
    unsigned char ibuf[] = "compute sha1";
    unsigned char obuf[20];

    SHA1(ibuf, strlen((char *)ibuf), obuf);

	for (int i=0; i<20; i++)
	{
		cout << hex << setfill('0') << setw(2) << (int)obuf[i] << " ";
	}
	cout << endl;

    return 0;
}
