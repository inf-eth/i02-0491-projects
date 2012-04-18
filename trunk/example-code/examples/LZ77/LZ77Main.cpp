#include <iostream>
#include <LZ77.h>

int main (int argc, char **argv)
{
	CLZ77 Test;
	Test.Encode("test.txt", "OutTest.txt");
	return 0;
}
