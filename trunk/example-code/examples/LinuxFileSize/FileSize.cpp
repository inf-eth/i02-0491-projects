// Default run determines size in bytes of file named 'FileSize.cpp'
#include <sys/stat.h>
#include <iostream>
using std::cout;
using std::endl;
int main (int argc, char **argv)
{
	struct stat filestats;
	stat ("FileSize.cpp", &filestats);
	cout << filestats.st_size << " bytes." << endl;
	return 0;
}
