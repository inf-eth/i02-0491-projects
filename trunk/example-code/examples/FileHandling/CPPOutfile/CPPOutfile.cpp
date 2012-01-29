#include <fstream>
using namespace std;

int main ( int argc, char **argv )
{

	fstream outfile;
	outfile.open ( "test1.txt", ios::out );
	for ( int i=0; i<10; i++ )
	{
		int x = rand ( );
		outfile << x;
	}
	outfile.close ( );
	return 0;

}
