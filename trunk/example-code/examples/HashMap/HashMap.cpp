#include <iostream>
#include <map>

using std::cout;
using std::endl;
using std::map;

int main (int argc, char **argv)
{
	map<char *, int> Test;
	map<char *, int>::iterator it;

	Test["e"] = 1;
	Test["b"] = 2;
	Test["c"] = 3;
	Test["d"] = 4;

	int count = 0;
	for (it = Test.begin(); it != Test.end(); it++, count++)
	{
		cout << it->second << " " << endl;
		if (count == 2)
			Test.erase(it);
	}

	return 0;
}
