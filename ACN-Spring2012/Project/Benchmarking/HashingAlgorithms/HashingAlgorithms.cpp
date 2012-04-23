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
bool memncmp (const char *, const char *, int);		// Byte stream compare
void memncpy (char *, const char *, int);			// Byte stream copy
bool memncmp32 (const char *, const char *, int);	// 32bit word stream comparison
void memncpy32 (const char *, const char *, int);	// 32bit word stream copy
bool memncmp64 (const char *, const char *, int);	// 64bit word stream comparison
void memncpy64 (const char *, const char *, int);	// 64bit word stream copy

#define INSERTIONS 1024*32
#define REPETITIONS 1024

vector<string> RandomStrings;
__int64 tStart, tEnd;

vector<char*> MD5_Hashes;
vector<char*> SHA1_Hashes;
vector<char*> SHA256_Hashes;
vector<int*> Rabin32_Hashes;
vector<char*> Rabin32_Hashes_Interleaved;
vector<long long*> Rabin64_Hashes;
vector<char*> Rabin64_Hashes_Interleaved;

int MD5Collisions = 0;
int SHA1Collisions = 0;
int Rabin32Collisions = 0;
int Rabin32ICollisions = 0;
int Rabin64Collisions = 0;
int Rabin64ICollisions = 0;

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
	RabinHashFunction32 rabin32_1 = RabinHashFunction32(0x4D96487B);
	RabinHashFunction32 rabin32_2 = RabinHashFunction32(0x2DC7EEB3);
	Rabin32 = rabin32_1.hash((const char*)ibuf, strlen((const char*)ibuf));
	cout << "Rabin: " << Rabin32 << dec << endl;

	// Rabin64 example.
	// 64 bit irreducible polynomials.
	// 1 7FABFBF6 5FFFFEFF
	// 1 01751A98 4D90AF27
	// 1 460C8808 10028043 slow
	// 1 7523C013 A96DD7FF fast
	// 1 77FFFFFF FFDFFFBF
	RabinHashFunction64 rabin64_1 = RabinHashFunction64(0x7523C013A96DD7FF);
	RabinHashFunction64 rabin64_2 = RabinHashFunction64(0x460C880810028043);
	Rabin64 = rabin64_1.hash((const char *)ibuf, strlen((const char *)ibuf));
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
			if (memncmp64(MD5_Hashes[i], (const char *)temp, 16) == true)
				MD5Collisions++;
			MD5_Hashes.push_back((char*)temp);
		}
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
			if (memncmp32(SHA1_Hashes[i], (const char *)temp, 20) == true)
				SHA1Collisions++;
			SHA1_Hashes.push_back((char*)temp);
		}
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
			*Rabintemp = rabin32_1.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
		for (int j=0; j<(int)Rabin32_Hashes.size(); j++)
		{
			if (*Rabintemp == *Rabin32_Hashes[j])
				Rabin32Collisions++;
			Rabin32_Hashes.push_back(Rabintemp);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " Rabin32 fingerprints: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "Rabin32 Collisions: " << Rabin32Collisions << endl;

	// Rabin32 interleaved test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		char* Key = new char[8];
		int Rabin32Hash1;
		int Rabin32Hash2;
		for (int z=0; z<REPETITIONS; z++)
		{
			Rabin32Hash1 = rabin32_1.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
			Rabin32Hash2 = rabin32_2.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
		}
		unsigned char SubKey1[4];
		unsigned char SubKey2[4];
		memncpy32((char *)SubKey1, (const char *)&Rabin32Hash1, 4);
		memncpy32((char *)SubKey2, (const char *)&Rabin32Hash2, 4);
		for (int x=0; x<4; x++)
		{
			Key[x*2] = SubKey1[x];
			Key[x*2+1] = SubKey2[x];
		}
		for (int j=0; j<(int)Rabin32_Hashes_Interleaved.size(); j++)
		{
			if (memncmp32(Key, Rabin32_Hashes_Interleaved[j], 8) == true)
				Rabin32ICollisions++;
			Rabin32_Hashes_Interleaved.push_back(Key);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " Rabin32 interleaved fingerprints: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "Rabin32I Collisions: " << Rabin32ICollisions << endl;

	// Rabin64 test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		long long *Rabin64temp = new long long;
		for (int z=0; z<REPETITIONS; z++)
			*Rabin64temp = rabin64_1.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
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

	// Rabin64 interleaved test.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		char* Key = new char[16];
		long long Rabin64Hash1;
		long long Rabin64Hash2;
		for (int z=0; z<REPETITIONS; z++)
		{
			Rabin64Hash1 = rabin64_1.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
			Rabin64Hash2 = rabin64_2.hash((const char*)RandomStrings[i].c_str(), RandomStrings[i].size());
		}
		unsigned char SubKey1[8];
		unsigned char SubKey2[8];
		memncpy32((char *)SubKey1, (const char *)&Rabin64Hash1, 8);
		memncpy32((char *)SubKey2, (const char *)&Rabin64Hash2, 8);
		for (int x=0; x<8; x++)
		{
			Key[x*2] = SubKey1[x];
			Key[x*2+1] = SubKey2[x];
		}
		for (int j=0; j<(int)Rabin64_Hashes_Interleaved.size(); j++)
		{
			if (memncmp32(Key, Rabin64_Hashes_Interleaved[j], 16) == true)
				Rabin64ICollisions++;
			Rabin64_Hashes_Interleaved.push_back(Key);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for computation of " << INSERTIONS << "x" << REPETITIONS << " Rabin64 interleaved fingerprints: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;
	cout << "Rabin64I Collisions: " << Rabin64ICollisions << endl;

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

// 32bit word comparison
bool memncmp32 (const char *block1, const char *block2, int size)
{
	for (int i=0; i<size/4; i++)
	{
		if (((unsigned int *)block1)[i] != ((unsigned int *)block2)[i])
			return false;
	}
	return true;
}

// 32bit word copy
void memncpy32 (const char *dst, const char *src, int size)
{
	for (int i=0; i<size/4; i++)
		((unsigned int *)dst)[i] = ((unsigned int *)src)[i];
}

// 64bit word comparison
bool memncmp64 (const char *block1, const char *block2, int size)
{
	for (int i=0; i<size/8; i++)
	{
		if (((unsigned long long *)block1)[i] != ((unsigned long long *)block2)[i])
			return false;
	}
	return true;
}

// 64bit word copy
void memncpy64 (const char *dst, const char *src, int size)
{
	for (int i=0; i<size/8; i++)
		((unsigned long long *)dst)[i] = ((unsigned long long *)src)[i];
}
