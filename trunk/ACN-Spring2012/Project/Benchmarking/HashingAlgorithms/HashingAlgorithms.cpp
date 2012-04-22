// Reference for hashing functions: http://stackoverflow.com/questions/918676/generate-sha-hash-in-openssl
// Reference for Rabin fingerprinting: http://www.xmailserver.org/rabin_impl.pdf

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <rabinhash32.h>
#include <rabinhash64.h>
#include <vector>
#include <string>
#include <Timing.h>

using std::cout;
using std::hex;
using std::dec;
using std::setfill;
using std::setw;
using std::endl;
using std::vector;
using std::string;

// Compare two blocks of memory and returns true if they match else returns false.
bool memncmp (const char *, const char *, int);
// Memcopy.
void memncpy (char *, const char *, int);

#define INSERTIONS 1024*32
#define REPETITIONS 1024

vector<string> RandomStrings;
__int64 tStart, tEnd;

vector<char*> MD5_Hashes;
vector<char*> SHA1_Hashes;
vector<char*> SHA256_Hashes;
vector<int*> Rabin_Hashes;
vector<long long*> Rabin64_Hashes;

int MD5Collisions = 0;
int SHA1Collisions = 0;
int RabinCollisions = 0;
int Rabin64Collisions = 0;

int main()
{
	unsigned char ibuf[] = "compute hash";
	unsigned char MD5obuf[16];
	unsigned char SHA1obuf[20];
	unsigned char SHA256obuf[32];
	int Rabin32;
	long long Rabin64;

	// MD5 example.
	MD5(ibuf, strlen((char *)ibuf), MD5obuf);

	cout << "MD5: ";
	for (int i=0; i<16; i++)
		cout << hex << setfill('0') << setw(2) << (int)MD5obuf[i];
	cout << endl;

	// Sha1 example.
	SHA1(ibuf, strlen((char *)ibuf), SHA1obuf);

	cout << "SHA1: ";
	for (int i=0; i<20; i++)
		cout << hex << setfill('0') << setw(2) << (int)SHA1obuf[i];
	cout << endl;

	// Sha256 example.
	SHA256(ibuf, strlen((char *)ibuf), SHA256obuf);

	cout << "SHA256: ";
	for (int i=0; i<32; i++)
		cout << hex << setfill('0') << setw(2) << (int)SHA256obuf[i];
	cout << endl;

	// Rabin32 example.
	// 32 bit irreducible polynomials.
	// 1 4D96487B
	// 1 2DC7EEB3
	// 1 1100C021
	// 1 53BCFEDB
	// 1 00401003
	RabinHashFunction32 rabin = RabinHashFunction32(1);
	Rabin32 = rabin.hash((const char*)ibuf, strlen((const char*)ibuf));
	cout << "Rabin: " << Rabin32 << dec << endl;

	// Rabin64 example.
	// 64 bit irreducible polynomials.
	// 1 7FABFBF6 5FFFFEFF
	// 1 01751A98 4D90AF27
	// 1 460C8808 10028043 slow
	// 1 7523C013 A96DD7FF fast
	// 1 77FFFFFF FFDFFFBF
	RabinHashFunction64 rabin1 = RabinHashFunction64(0x7523C013A96DD7FF);
	Rabin64 = rabin1.hash((const char *)ibuf, strlen((const char *)ibuf));
	cout << "Rabin64: " << (unsigned long long)Rabin64 << dec << endl;

	// Random Unique string generation.
	srand((unsigned int)time(NULL));

	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		int size = rand()%48+2;
		char *temp = new char[size];
		for (int j=0; j<size; j++)
			temp[j] = (char)(rand()%25+65);
		temp[size-1] = '\0';
		bool Insert = true;
		for (int j=0; j<(int)RandomStrings.size(); j++)
		{
			if (RandomStrings[j] == temp)
			{
				Insert = false;
				i--;
				break;
			}
		}
		if (Insert == true)
			RandomStrings.push_back(temp);
		delete []temp;
	}
	tEnd = GetTimeus64();
	cout << "Time taken for random generation of " << INSERTIONS << " strings: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	// MD5 test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		unsigned char *temp = new unsigned char[16];
		for (int z=0; z<REPETITIONS; z++)
			MD5((const unsigned char *)RandomStrings[i].c_str(), RandomStrings[i].size(), temp);
		for (int j=0; j<(int)MD5_Hashes.size(); j++)
		{
			if (memncmp(MD5_Hashes[i], (const char *)temp, 16) == true)
				MD5Collisions++;
			MD5_Hashes.push_back((char*)temp);
		}
		//delete []temp;
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " MD5 hashes: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "MD5 collisions: " << MD5Collisions << endl;

	// SHA1 test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		unsigned char *temp = new unsigned char[20];
		for (int z=0; z<REPETITIONS; z++)
			SHA1((const unsigned char *)RandomStrings[i].c_str(), RandomStrings[i].size(), temp);
		for (int j=0; j<(int)SHA1_Hashes.size(); j++)
		{
			if (memncmp(SHA1_Hashes[i], (const char *)temp, 20) == true)
				SHA1Collisions++;
			SHA1_Hashes.push_back((char*)temp);
		}
		//delete []temp;
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " SHA1 hashes: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "SHA1 collisions: " << SHA1Collisions << endl;

	// Rabin test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		int *Rabintemp = new int;
		for (int z=0; z<REPETITIONS; z++)
			*Rabintemp = rabin.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
		for (int j=0; j<(int)Rabin_Hashes.size(); j++)
		{
			if (*Rabintemp == *Rabin_Hashes[j])
				RabinCollisions++;
			Rabin_Hashes.push_back(Rabintemp);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " Rabin fingerprints: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "Rabin Collisions: " << RabinCollisions << endl;

	// Rabin64 test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		long long *Rabin64temp = new long long;
		for (int z=0; z<REPETITIONS; z++)
			*Rabin64temp = rabin1.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
		for (int j=0; j<(int)Rabin64_Hashes.size(); j++)
		{
			if (*Rabin64temp == *Rabin64_Hashes[j])
				Rabin64Collisions++;
			Rabin64_Hashes.push_back(Rabin64temp);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " Rabin64 fingerprints: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "Rabin64 Collisions: " << Rabin64Collisions << endl;

	return 0;

}

// Compare two blocks of memory and returns true if they match else returns false.
bool memncmp (const char *block1, const char *block2, int size)
{
	for (int i=0; i<size; i++)
	{
		if (block1[i] != block2[i])
			return false;
	}
	return true;
}

void memncpy (char *dst, const char *src, int size)
{
	for (int i=0; i<size; i++)
		dst[i] = src[i];
}

