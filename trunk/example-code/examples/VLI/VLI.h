#include <vector>
#include <iostream>
using std::ostream;
using std::istream;
using std::vector;
class CVLI
{
	bool Sign;	// 0=positive, 1=negative
	vector<unsigned short> Number;

	public:
	CVLI ();

	// Input and output.
	friend ostream& operator << (ostream&, const CVLI&);
	friend istream& operator >> (istream&, CVLI&);
};
