#include <fcntl.h>
#include <iostream>
using namespace std;

int main ( int argc, char **argv )
{
	int fd = open ( "data.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU );
	cout << "fd = " << fd << endl;
	int bytes;
	for ( int i=0; i <10; i++ )
	{
		int x = rand ( );
		int bytes = write ( fd, (void *)&x, 4 );
		cout << "bytes = " << bytes << endl;
		cout << "data = " << x << endl;
	}
	close ( fd );

	fd = open ( "data.txt", O_RDONLY );
	for ( int i=0; i <10; i++ )
	{
		char buff[4];
		int *z = (int *)&buff;
		int bytes = read ( fd, buff, 4 );
		cout << "bytes read = " << bytes << endl;
		cout << "data = " << *z << endl;
	}
	close ( fd );
	return 0;
}
