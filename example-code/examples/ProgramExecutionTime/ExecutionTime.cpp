// Reference: http://stackoverflow.com/questions/2215744/c-programming-how-can-i-get-execution-time-of-a-program-in-milliseconds
// gprof, which is part of the GNU toolkit, is an option. 
// Most POSIX systems will have it installed, and it's available under Cygwin for Windows. 
// Tracking the time yourself using gettimeofday() works fine, but it's the performance 
// equivalent of using print statements for debugging. It's good if you just want a quick 
// and dirty solution, but it's not quite as elegant as using proper tools.
// To use gprof, you must specify the -pg option when compiling with gcc as in:

// gcc -o prg source.c -pg

//Then you can run gprof on the generated program as follows:

// gprof prg > gprof.out

// By default, gprof will generate the overall runtime of your program, as well as the 
// amount of time spent in each function, the number of times each function was called,
// the average time spent in each function call, and similar information.

// There are a large number of options you can set with gprof. If you're interested,
// there is more information in the man pages or through Google.

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctime>
#define __int64 long long
#endif

#include "ExecutionTime.h"
#include <iostream>
#include <time.h>
using std::cout;
using std::endl;

/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */

__int64 GetTimeMs64()
{
#ifdef WIN32
 /* Windows */
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. */
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 __int64 ret = li.QuadPart;
 ret -= 116444736000000000L; /* Convert from file time to UNIX epoch time. */
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

 return ret;
#else
 /* Linux */
 struct timeval tv;

 gettimeofday(&tv, NULL);

 uint64_t ret = tv.tv_usec;
 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
 ret /= 1000;

 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
 ret += (tv.tv_sec * 1000);

 return ret;
#endif
}

int main(void) 
{
    clock_t tStart = clock();

	// Do something here.

	clock_t tEnd = clock();
    
    //printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	cout << (double)(tEnd - tStart)/CLOCKS_PER_SEC << endl;
    return 0;
}
