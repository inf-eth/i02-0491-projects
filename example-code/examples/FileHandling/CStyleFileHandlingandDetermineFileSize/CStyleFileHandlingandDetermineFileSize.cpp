#include <cstdio>

int main( void )
{
   FILE *stream;
   char line[81];
   int  result;

   // Creates a file named fseek.out if it doesn't exist and write something in it. Otherwise just overwrite.
	stream = fopen( "fseek.out", "w+" );

	// Write a message on the file.
   fprintf( stream, "The fseek begins here: ""This is the file 'fseek.out'.\n" );

   // Move the seek pointer in the file to middle.
	fseek( stream, 23L, SEEK_SET);		// Seek set positions the seek pointer at start of file. 23 is the offset from start which ends up in the middle.

   printf( "File pointer is set to middle of first line.\n" );

   // Read from the file. Seek pointer is in the middle of the file.
   fgets( line, 80, stream );
      printf( "%s", line );

   // Determine file size.
   result = fseek( stream, 0L, SEEK_END);		// Position the pointer at end of file.
   result = ftell (stream);						// Tell the location of pointer.
   printf ("File size = %d", result);

   fclose( stream );
}
