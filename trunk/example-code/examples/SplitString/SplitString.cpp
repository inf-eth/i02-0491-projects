// Reference: Taken from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c?rq=1
// Example Usage:
// vector<string> v = split<string>("Hello, there; World", ";,");
// vector<wstring> v = split<wstring>(L"Hello, there; World", L";,");

#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<typename T> vector<T> split(const T & str, const T & delimiters)
{
	vector<T> v;
	typename T::size_type start = 0;
	typename T::size_type pos = str.find_first_of(delimiters, start);
	while(pos != T::npos)
	{
		if(pos != start) // ignore empty tokens
			v.push_back(str.substr(start, pos - start));
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}
	if(start < str.length()) // ignore trailing delimiter
		v.push_back(str.substr(start, str.length() - start)); // add what's left of the string
	return v;
}

int main()
{
	string s;
	string delim;

	cout << "Enter a string to split: ";
	getline(cin, s);

	cout << "Enter delimiters: ";
	getline(cin, delim);

	vector<string> v = split<string>(s, delim);

	cout << "Tokens are: " << endl;
	for (unsigned int i=0; i<v.size(); i++)
		cout << v[i] << endl;

	return 0;
}
