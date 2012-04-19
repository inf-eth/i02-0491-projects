#include <cstring>
#include <iostream>
#include <LZ77.h>

using std::cerr;
using std::endl;

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		cerr << "USAGE: " << endl
			 << "Encode: ./LZ77 -e <input file> <output file>" << endl
			 << "Decode: ./LZ77 -d <input file> <output file>" << endl;
		return -1;
	}

	CLZ77 Test;
	if (strcmp(argv[1], "-e") == 0)
		Test.Encode(argv[2], argv[3]);
	else if (strcmp(argv[1], "-d") == 0)
		Test.Decode(argv[2], argv[3]);
	else
	{
		cerr << "ERROR: Unexpected error. Check arguments and input files." << endl;
		return -1;
	}
	return 0;
}
