#include <RadixTree.h>
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	CRadixTree Tree;
	Tree.CreateTree();
	int *paytest = new int;
	*paytest = 3;
	int *paysearch = NULL;
	int ComparisonCounter = 0;
	Tree.Insert("testword", strlen("testword"), (void *)paytest);
	paysearch = (int*)Tree.Search("testword", strlen("testword"), ComparisonCounter);
	if (paysearch == NULL)
		cout << "Search unsuccessful." << endl;
	else
		cout << "Found value of testword: " << *paysearch << endl;
	Tree.ResetTree();
	return 0;
}
