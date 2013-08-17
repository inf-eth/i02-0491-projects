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
	printf ("File size = %d\n", result);

	fclose( stream );

	/*
	FILE *outstream;
	FILE *instream;
	char line[81]; // This string holds the data read from file.
	int Age;
	float GPA;

	outstream = fopen("StudentData.txt", "w+");
	fprintf(outstream, "====== Student Data ======\n"); // Writing something onto file.
	fprintf(outstream, "Name: This Is My Name\n"); // Writing something onto file.
	fprintf(outstream, "Age: %d\n", 21);
	fprintf(outstream, "GPA: %f\n", 3.21);
	fprintf(outstream, "===========================\n");
	fclose(outstream);  // Close the out put file.

	instream = fopen("StudentData.txt", "r"); // Open the file for reading only.
	fgets(line, 80, instream); // Read a line and store in 'line'.
	printf("%s", line); // Display on console.
	fgets(line, 80, instream); // Read another line.
	printf("%s", line); // Display on console.
	fscanf(instream, "Age: %d\n", &Age); // Extra characters in scanf are only for positioning.
	printf("Age: %d\n", Age);
	fscanf(instream, "GPA: %f\n", &GPA); // Extra characters in scanf are only for positioning.
	printf("GPA: %f\n", GPA);
	fgets(line, 80, instream); // Read another line.
	puts(line); // Display on console.
	fclose(instream);  // Close the out put file.
	*/

	return 0;
}
