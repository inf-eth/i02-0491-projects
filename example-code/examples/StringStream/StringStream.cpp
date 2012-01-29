// Refrence: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/

#include <iostream>
#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

int main (int argc, char **argv)
{
	int x = 2;
	float y = 3.2f;
	std::stringstream xystringstream;
	xystringstream << "x=" << x << " y=" << y;
	std::cout << xystringstream.str() << std::endl;
	return 0;
}
