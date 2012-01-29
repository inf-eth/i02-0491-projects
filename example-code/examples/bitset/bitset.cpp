#include <iostream>
#include <bitset>
using std::cout;
using std::endl;
using std::bitset;
int main (int argc, char **argv)
{
	int z = 3;
	int *zptr = &z;
	// Printing stored address (of z).
	printf ("address of z = %x\n", zptr);
	// Assigning a random address 16 = 10 in hex.
	zptr = (int *)16;
	printf ("Random address = %x\n", zptr);

	int x = 0;
	float y = -6.25f;
	unsigned char *xp = (unsigned char *)&x;

	// Displaying the int in binary.
	cout << "x bits = " << bitset<CHAR_BIT*sizeof(int)>(x) << endl;		// Bad way to print x's bits lol1!

	// Changing first byte of int to 3.
	*xp = 3;
	cout << "x = " << x << endl;
	cout << bitset<CHAR_BIT>( *xp ) << endl;

	// Changing second byte of int to 3.
	*(xp+1) = 3;
	cout << "x = " << x << endl;
	cout << bitset<CHAR_BIT>( *xp ) << endl;

	// Displaying the int in binary
	cout << "x bits = " << bitset<CHAR_BIT*sizeof(int)>(x) << endl;

	// We can even do this with the float.
	xp = (char unsigned *)&y;
	cout << "y = " << y << endl;
	// Better ways to display float but well this one works.
	cout << "y bits = " << bitset<CHAR_BIT>( *xp ) << bitset<CHAR_BIT>( *xp+1 ) << bitset<CHAR_BIT>( *xp+2 ) << bitset<CHAR_BIT>( *xp+3 ) << endl;
	// Another way to display.
	bitset <32> FloatBits;
	union stuffk
	{
		float fff;
		int intlol;
	} kkkj;

	kkkj.fff = -6.25f;
	FloatBits |= kkkj.intlol;
	cout << "Float bits using bitset = " << FloatBits << endl;

	bitset <64> DoubleBits;
	union stuff
	{
		double AnyDouble;
		long DoubleLonglol;
	} kkk;

	kkk.AnyDouble = -6.25;

	DoubleBits |= kkk.DoubleLonglol;		// Copying Double's bits into bitset. Can't use Double directly or it will only copy the decimal part truncating the fractional part. Casting lol!?
	cout << "3.3 double = " << kkk.AnyDouble << ", bits = " << DoubleBits << endl;
	xp = (unsigned char *)&kkk.AnyDouble;
	cout << "Double bits as char = " << bitset<CHAR_BIT>( *xp ) << bitset<CHAR_BIT>( *(xp+1) ) << bitset<CHAR_BIT>( *(xp+2) ) << bitset<CHAR_BIT>( *(xp+3) ) << bitset<CHAR_BIT>( *(xp+4) ) << bitset<CHAR_BIT>( *(xp+5) ) << bitset<CHAR_BIT>( *(xp+6) ) << bitset<CHAR_BIT>( *(xp+7) ) << endl ;

	return 0;
}