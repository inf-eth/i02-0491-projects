#include <cstring>
#include <iostream>
#include <LZ77.h>
#include <Timer.h>

using std::cerr;
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
	CLZ77 Test;
	/*
	Test.Encode("test.txt", "test.encoded.txt");
	Test.Decode("test.encoded.txt", "test.decoded.txt");
	return 0;
	*/
	if (argc != 4)
	{
		cerr << "USAGE: " << endl
			 << "Encode: ./LZ77 -e <input file> <output file>" << endl
			 << "Decode: ./LZ77 -d <input file> <output file>" << endl;
		return -1;
	}

	__int64 tStart, tEnd;
	tStart = GetTimeus64();

	if (strcmp(argv[1], "-e") == 0)
		Test.Encode(argv[2], argv[3]);
	else if (strcmp(argv[1], "-d") == 0)
		Test.Decode(argv[2], argv[3]);
	else
	{
		cerr << "ERROR: Unexpected error. Check arguments and input files." << endl;
		return -1;
	}

	tEnd = GetTimeus64();
	cout << "Time taken = " << ((double)(tEnd-tStart))/(1000000.) << " seconds." << endl;

	return 0;
}
