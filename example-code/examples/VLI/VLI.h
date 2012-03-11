#include <vector>
#include <iostream>
using std::vector;
using std::ostream;
using std::istream;

class CVLI
{
	bool Sign;			// 0=positive, 1=negative
	vector<short> Number;
	void Format ();		// Properly format the Number.
	bool IsZero () const;		// Is this zero?
	CVLI Abs () const;

	public:
	CVLI ();
	CVLI::CVLI (bool, vector<short> &);		// VLI from sign and number.

	// Check for primality.
	bool CheckPrime (bool=false);

	// Comparison operators.
	bool operator == (const CVLI&);
	bool operator != (const CVLI&);
	bool operator < (const CVLI&);
	bool operator > (const CVLI&);
	bool operator <= (const CVLI&);
	bool operator >= (const CVLI&);

	// Pre-post increment.
	CVLI operator ++ ();
	CVLI operator ++ (int);

	// Addition and Subtraction.
	CVLI operator + (const CVLI&);
	CVLI operator - (const CVLI&);

	// Multiplication and division.
	CVLI operator * (const CVLI&);
	CVLI operator / (const CVLI&);

	// modulus
	CVLI operator % (const CVLI&);

	// Input and output.
	friend ostream& operator << (ostream&, const CVLI&);
	friend istream& operator >> (istream&, CVLI&);
};
