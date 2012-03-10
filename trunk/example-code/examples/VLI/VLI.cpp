#include "VLI.h"
#include <string>
#include <cstdlib>
using std::string;

CVLI::CVLI (): Sign(0)
{
	
}

// << and >> operators.
ostream& operator<<(ostream& out, const CVLI& pVLI)
{
    out << (pVLI.Sign?"-":"");
	for(int i= pVLI.Number.size()-1; i >= 0; i--) 
        out << pVLI.Number[i];
    return out;
}
istream& operator >> (istream& in, CVLI& pVLI)
{
	string input;
	in >> input;

	// Sign check.
	if (input.size() > 0 && input[0]=='-')
	{
		pVLI.Sign = 1;
		input.replace (0, 1, "");
		//std::cout << "...." << std::endl;
	}
	else
		pVLI.Sign = 0;

	// Remove any preceding zeros.
	for (int i=pVLI.Sign; i < input.size(); i++)
	{
		if (input[0] == '0')
			input.replace (0, 1, "");
		else
			break;
	}

	for (int i=input.size()-1; i >= 0; i--)
	{
		//std::cout << "input[" << i << "] = " << (unsigned short)(input.c_str()[i]-48) << std::endl;
		if ((unsigned short)(input.c_str()[i]-48) >= 0U && (unsigned short)(input.c_str()[i]-48) <= 9U)
			pVLI.Number.push_back ((unsigned short)(input.c_str()[i]-48));
		else
			continue;
	}
	return in;
}