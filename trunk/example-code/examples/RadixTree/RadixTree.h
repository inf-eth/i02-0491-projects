// RadixTree.h: interface for the CRadixTree class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>

// The Standard Radix Tree Node.
struct RadixTreeNode
{
	char Label;
	short NoOfNodes;
	short Status;
	short Width;
	void* Payload;

	// Pointers.
	RadixTreeNode *ParentPTR;
	RadixTreeNode *NextNodePTRS[256];
};

// *** The Radix Tree Dictionary Class Definition ***
class CRadixTree
{
public:
	RadixTreeNode *RootPTR;

	// *** Constructor/Destructor ***
	CRadixTree();
	virtual ~CRadixTree();

	// ************************ Public Functions **************************
	void CreateTree();
	void ResetTree();
	
	// *** Dictionary Functions ***
	int Insert(const char *, int, void *);
	void* Search(const char *, int, int&);
	int Delete(const char *, int);
	// ****************************
	// ********************************************************************
private:
	int NoOfNodes;

	// *** Private Mutator ( Whatever ) Functions ***
	void SetWidthForNewNode(RadixTreeNode*);
	RadixTreeNode* GetMaximumNodeChild(RadixTreeNode*);
	int  CalculateWidth(RadixTreeNode*);

	void DeleteTree(RadixTreeNode*);
	void DeleteNode(RadixTreeNode*);
	// **********************************************
	/*int Width;*/	// Maybe I will need this. Maybe not.
	/*int Height*/	// Same as above.
};
