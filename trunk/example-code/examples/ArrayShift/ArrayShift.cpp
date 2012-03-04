#define ARRAYSIZE	6
#define RANGE		100
#define SHIFT		1

#include <ctime>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

void PrintArray (int *, int);
void PopulateArray (int *, int, int);
void ShiftArray (int *, int, int);

int main (int argc, char* argv[])
{
	srand ((unsigned int)time (NULL));
	int Array[ARRAYSIZE];
	
	PopulateArray (Array, ARRAYSIZE, RANGE);
	cout << "Displaying original array: " << endl;
	PrintArray (Array, ARRAYSIZE);

	ShiftArray (Array, ARRAYSIZE, SHIFT);

	cout << "Displaying shifted array: " << endl;
	PrintArray (Array, ARRAYSIZE);

	return 0;
}

void PrintArray (int pArray[], int size)
{
	for (int i=0; i<size; i++)
	{
		cout << "Array[" << i << "] = " << pArray[i] << endl;
	}
}

void PopulateArray (int pArray[], int size, int range)
{
	for (int i=0; i<size; i++)
	{
		pArray[i] = i;//rand() % range;
	}
}

void ShiftArray (int pArray[], int size, int shift)
{
	int temp1 = pArray[0];
	int temp2;
	int SwapIndex = 0;
	//bool evenfactor = !(size%2);
	//bool wraparoundflag = 0;
	//int wrapcount = 0;
	//cout << "Even factor = " << evenfactor << endl;

	for (int i=0; i<size; i++)
	{
		// Swap.
		temp2 = pArray[(SwapIndex+shift)%size];//+(wraparoundflag*evenfactor*(SwapIndex+shift)/size)];
		pArray[(SwapIndex+shift)%size] = temp1;//+(wraparoundflag*evenfactor*(SwapIndex+shift)/size)] = temp1;
		temp1 = temp2;
		
		//if (wraparoundflag == true)
		//{
			//SwapIndex++;
			//SwapIndex = SwapIndex - shift;
			//wraparoundflag = false;
		//}

		//if ((SwapIndex+shift) / size == 1)
			//wraparoundflag = true;

		SwapIndex = (SwapIndex+shift)%size;//+(wraparoundflag*evenfactor*(SwapIndex+shift)/size);

		cout << "Swap Index = " << SwapIndex << endl;
	}
}