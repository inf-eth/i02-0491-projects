#include <RadixTree.h>
#include <Timing.h>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <openssl/sha.h>

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

#define KEY_LENGTH 20
#define INSERTIONS 1024*16

struct ContentPrevalenceEntry
{
	unsigned char Key[KEY_LENGTH];
	int Count;
	__int64 InsertionTime;
};

// Functions.
unsigned char * GenerateKey (unsigned char *, unsigned int);
int SearchContentPrevalenceTable (unsigned char *);
// Compare two blocks of memory and returns true if they match else returns false.
bool memncmp (const char *, const char *, int);
// Memcopy.
void memncpy (char *, const char *, int);

// Globals.
vector<ContentPrevalenceEntry> ContentPrevalenceTable;
CRadixTree<int> Tree;
__int64 tStart, tEnd;
vector<string> RandomStrings;
vector<unsigned char*> Keys;

int main (int argc, char **argv)
{
	// Random string generation.
	srand((unsigned int)time(NULL));

	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		int size = rand()%48+2;
		char *temp = new char[size];
		for (int j=0; j<size; j++)
			temp[j] = (char)(rand()%25+65);
		temp[size-1] = '\0';
		RandomStrings.push_back(temp);
		delete []temp;
	}
	tEnd = GetTimeus64();
	cout << "Time taken for random generation of " << INSERTIONS << " strings: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	// Key generation.
	tStart = GetTimeus64();
	for (int i=0; i<INSERTIONS; i++)
	{
		unsigned char *temp = GenerateKey((unsigned char *)RandomStrings[i].c_str(), (unsigned int)RandomStrings[i].size());
		Keys.push_back(temp);
	}
	tEnd = GetTimeus64();
	cout << "Time taken for random generation of " << INSERTIONS << " keys: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	// Unique insertions in Address Dispersion Table vector.
	tStart = GetTimeus64();
	int SearchIndex;
	ContentPrevalenceEntry temp;
	for (int i=0; i<INSERTIONS; i++)
	{
		SearchIndex = SearchContentPrevalenceTable (Keys[i]);
		if (SearchIndex != -1)
			ContentPrevalenceTable[SearchIndex].Count++;
		else
		{
			memncpy((char *)temp.Key, (const char *)Keys[i], KEY_LENGTH);
			temp.InsertionTime = GetTimeus64();
			temp.Count = 1;
			ContentPrevalenceTable.push_back(temp);
		}
	}
	tEnd = GetTimeus64();
	cout << "Time taken for " << INSERTIONS << " insertions in vector: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	// Unique insertions in Radix Tree.
	tStart = GetTimeus64();
	Tree.CreateTree();
	int *RadixSearch;
	for (int i=0; i<INSERTIONS; i++)
	{
		int ComparisonCounter = 0;
		RadixSearch = Tree.Search((const unsigned char *)Keys[i], KEY_LENGTH, ComparisonCounter);
		if (RadixSearch == NULL)
		{
			int Count = 1;
			Tree.Insert((const unsigned char *)Keys[i], KEY_LENGTH, Count);
		}
		else
			(*RadixSearch)++;
	}
	tEnd = GetTimeus64();
	cout << "Time taken for " << INSERTIONS << " insertions in radix tree: " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	return 0;
}

// Search Key in Content Prevalence Table.
int SearchContentPrevalenceTable (unsigned char *pKey)
{
	for (int i=0; i < (int)ContentPrevalenceTable.size(); i++)
	{
		if (memncmp ((const char *)ContentPrevalenceTable[i].Key, (const char *)pKey, KEY_LENGTH) == true)
			return i;
	}
	return -1;
}

// Generate Key (sha1 hash).
unsigned char * GenerateKey (unsigned char *ppayload, unsigned int psize_payload)
{
	unsigned char *Key = new unsigned char[KEY_LENGTH];
	unsigned char *Input = new unsigned char[psize_payload];

	SHA1(ppayload, psize_payload, Key);
	return Key;
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
