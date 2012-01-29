#include <cstdio>
#include <cstdlib>

struct timemachine
{
	char name[15];
	int number;
	float volts;
};

int main ()
{
	timemachine *mymachine = NULL;
	mymachine = (timemachine *) malloc (23);
	
	printf ("input attriutes...\n");
	gets(mymachine->name );
	mymachine->number = 3;
	mymachine->volts = 12.f;

	printf ("mymachine attributes are: \n");
	puts (mymachine->name);
	printf ("num: %d, volts: %f\n", mymachine->number, mymachine->volts);

	if (mymachine != NULL) free(mymachine);
	return 0;
}
