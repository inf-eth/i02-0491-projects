#include "timer.h"
#include <iostream>

int main (int argc, char **argv)
{
	__int64 ms = GetTimeMs64();
	__int64 us = GetTimeus64();
	std::cout << "ms = " << ms << std::endl;
	std::cout << "us = " << us << std::endl;

	return 0;
}
