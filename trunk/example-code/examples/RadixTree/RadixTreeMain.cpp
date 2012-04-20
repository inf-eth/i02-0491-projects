#include <RadixTree.h>
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	CRadixTree<int> Tree;
	Tree.CreateTree();

	int paytest = 3;
	int *paysearch;
	int ComparisonCounter = 0;

	Tree.Insert((const unsigned char *)"testword", strlen("testword"), paytest);
	paysearch = Tree.Search((const unsigned char *)"testword", strlen("testword"), ComparisonCounter);

	if (paysearch == NULL)
		cout << "Search unsuccessful." << endl;
	else
		cout << "Found value of testword: " << *paysearch << endl;

	Tree.ResetTree();
	return 0;
}
