#include <cstdio>
#ifdef WIN32
#include <windows.h>	// For Sleep(ms); Sleeps for indicated milliseconds.
#else
#include <time.h>		// Linux header for nanosleep().
#endif

int main (int argc, char *argv[])
{
	int ms = 0;
	int sec = 0;
	int min = 0;

	#ifndef WIN32
	timespec timeobj;
	timeobj.tv_sec = 0;
	timeobj.tv_nsec = 10*1000*1000;		// 10 milliseconds.
	#endif

	while (true)
	{
		// Pause for 10 milliseconds.
		#ifdef WIN32
		Sleep(10);
		#else
		nanosleep (&timeobj, NULL);
		#endif
		// Increment milliseconds variable.
		if (ms==900)
		{
			// if millisecond timer is at 999 reset ms timer and increment seconds.
			ms = 0;
			if (sec == 59)
			{
				// if second timer is at 59, reset it and increment min timer and beep.
				sec = 0;
				min++;
				printf ("/a");
			}
			else
				sec++;

		}
		else
			ms+=10;

		// Display time after every 10 milliseconds.
		printf ("\r\t\t\t\r");	// Clear screen.
		printf ("%2d:%2d:%3d", min, sec, ms/10);

	}
	return 0;
}
