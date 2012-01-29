#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

float f (float);
float Integrate (float, float, float);

int main (int argc, char **argv)
{
	float LowerLimit;
	float UpperLimit;
	float dx;

	cout << "Lower Limit: ";
	cin >> LowerLimit;
	cout << "Upper Limit: ";
	cin >> UpperLimit;
	cout << "dx: ";
	cin >> dx;

	cout << "Area = " << Integrate (LowerLimit, UpperLimit, dx) << endl;
	return 0;
}

float f (float x)
{
	return x*x;
}

float Integrate (float LL, float UL, float dx)
{
	float Integrand = 0;
	float x = LL;

	while (x <= UL)
	{
		Integrand = Integrand + f(x)*dx;
		x = x+dx;
	}
	return Integrand;
}
