#include <iostream>
#include <LZ77.h>

int main (int argc, char **argv)
{
	CLZ77 Test;
	Test.Encode("test.txt", "test.encoded.txt");
	Test.Decode("test.encoded.txt", "test.decoded.txt");
	return 0;
}
