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
	unsigned char obuf256[32];

	// Sha1 example.
    SHA1(ibuf, strlen((char *)ibuf), obuf);

	for (int i=0; i<20; i++)
	{
		cout << hex << setfill('0') << setw(2) << (int)obuf[i] << " ";
	}
	cout << endl;

	// Sha256 example.
	SHA256(ibuf, strlen((char *)ibuf), obuf256);

	for (int i=0; i<32; i++)
	{
		cout << hex << setfill('0') << setw(2) << (int)obuf256[i] << " ";
	}
	cout << endl;
	
    return 0;
}
