// RadixTree.h: interface for the CRadixTree class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>
#pragma once

// The Standard Radix Tree Node.
template<class T> class RadixTreeNode
{
public:
	char Label;
	short NoOfNodes;
	short Status;
	short Width;
	T Payload;

	// Pointers.
	RadixTreeNode<T> *ParentPTR;
	RadixTreeNode<T> *NextNodePTRS[256];
};

// *** The Radix Tree Dictionary Class Definition ***
template<class T> class CRadixTree
{
public:
	RadixTreeNode<T> *RootPTR;

	// *** Constructor/Destructor ***
	CRadixTree();
	virtual ~CRadixTree();

	// ************************ Public Functions **************************
	void CreateTree();
	void ResetTree();
	
	// *** Dictionary Functions ***
	int Insert(const char *, int, T&);
	T* Search(const char *, int, int&);
	int Delete(const char *, int);
	// ****************************
	// ********************************************************************
private:
	int NoOfNodes;

	// *** Private Mutator ( Whatever ) Functions ***
	void SetWidthForNewNode(RadixTreeNode<T> *);
	RadixTreeNode<T>* GetMaximumNodeChild(RadixTreeNode<T> *);
	int  CalculateWidth(RadixTreeNode<T> *);

	void DeleteTree(RadixTreeNode<T> *);
	void DeleteNode(RadixTreeNode<T> *);
	// **********************************************
	/*int Width;*/	// Maybe I will need this. Maybe not.
	/*int Height*/	// Same as above.
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class T> CRadixTree<T>::CRadixTree()
{
	RootPTR = NULL;
	NoOfNodes = 0;
}

template<class T> CRadixTree<T>::~CRadixTree()
{
	DeleteTree(RootPTR);
	RootPTR = NULL;
	NoOfNodes = 0;
}

// *********************************************************************************
// ****************** Radix Tree Dictionary's Function Implementation **************
// *********************************************************************************

template<class T> void CRadixTree<T>::CreateTree()
{
	RootPTR = new RadixTreeNode<T>;

	(*RootPTR).Label = 'R';
	(*RootPTR).NoOfNodes = 0;
	(*RootPTR).Status = -1;
	(*RootPTR).ParentPTR = NULL;
	(*RootPTR).Width = 1;

	for (int i=0; i<256; i++)
		(*RootPTR).NextNodePTRS[i] = NULL;

	NoOfNodes = 0;
}

// *** Insertion in the Radix Tree ***
template<class T> int CRadixTree<T>::Insert(const char *WordArray, int Length, T &Payload)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength ( );
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer ( 256 );
	RadixTreeNode<T> *TempPTR = RootPTR;

	for (int i=0; i<Length; i++)
	{
		if (i == (Length-1))
		{
			if ((*TempPTR).NextNodePTRS[(int)WordArray[i]] == NULL)
			{
				RadixTreeNode<T> *NewNode = new RadixTreeNode<T>;
				(*NewNode).Label = WordArray[i];
				(*NewNode).NoOfNodes = 0;
				(*NewNode).Status = 1;
				(*NewNode).Payload = Payload;
				(*NewNode).ParentPTR = TempPTR;
				(*TempPTR).NoOfNodes++;

				for (int j=0; j<256; j++)
					(*NewNode).NextNodePTRS[j] = NULL;

				(*TempPTR).NextNodePTRS[(int)WordArray[i]] = NewNode;
				SetWidthForNewNode(NewNode);
				NoOfNodes++;

				if ((*(*NewNode).ParentPTR).Status == 1)
					( *(*NewNode).ParentPTR).Status = 2;

				return 0;
			}
			else
			{
				(*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Payload = Payload;
				if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Status == 0 )
					(*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Status = 2;

				return 0;
			}
		}
		else
		{
			if ((*TempPTR).NextNodePTRS[(int)WordArray[i]] == NULL)
			{
				RadixTreeNode<T> *NewNode = new RadixTreeNode<T>;
				(*NewNode).Label = WordArray[i];

				(*NewNode).NoOfNodes = 0;
				(*NewNode).Status = 0;
				(*NewNode).ParentPTR = TempPTR;
				(*TempPTR).NoOfNodes++;
				
				for (int k=0; k<256; k++)
					(*NewNode).NextNodePTRS[k] = NULL;

				(*TempPTR).NextNodePTRS[(int)WordArray[i]] = NewNode;
				SetWidthForNewNode(NewNode);
				NoOfNodes++;
				
				TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];

				if ((*(*NewNode).ParentPTR).Status == 1)
					(*(*NewNode).ParentPTR).Status = 2;
			}
			else
				TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];
		}
	}
	return -1;
}

// *** Search in the Radix Tree ***
template<class T> T* CRadixTree<T>::Search(const char* WordArray, int Length, int &ComparisonCounter)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength();
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer(256);
	RadixTreeNode<T> *TempPTR = RootPTR;

	for (int i=0; i<Length; i++)
	{
		ComparisonCounter++;
		if ((*TempPTR).NextNodePTRS[(int)WordArray[i]] == NULL)
			return NULL;

		if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Label != WordArray[i])
			return NULL;

		if (i == Length-1)
		{
			if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Status == 1 || (*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Status == 2)
				return &(*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Payload;
		}

		if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Label == WordArray[i])
			TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];
		else
			return NULL;
	}
	return NULL;
}

// *** Deletion in the Radix Tree ***
template<class T> int CRadixTree<T>::Delete(const char* WordArray, int Length)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength ( );
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer ( 256 );
	RadixTreeNode<T> *TempPTR = RootPTR;

	for (int i=0; i<Length; i++)
	{
		if ((*TempPTR).NextNodePTRS[(int)WordArray[i]] == NULL)
			return -1;

		if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Label != WordArray[i])
			return -1;

		if (i == Length-1)
		{
			if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Status == 1 || (*(*TempPTR).NextNodePTRS[(int)WordArray[i]] ).Status == 2)
			{
				/*
				Call the delete this node function
				*/
				RadixTreeNode<T> *DELTempPTR;
				TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];

				if ( (*TempPTR).Status == 2 )
				{
					if ((*TempPTR).Payload != NULL)
						//delete (*TempPTR).Payload;
					(*TempPTR).Payload = NULL;
					(*TempPTR).Status = 0;
					return 0;
				}
				else if ((*TempPTR).Status == 1)
				{
					while (TempPTR != RootPTR && (*TempPTR).Status != 2 && (*TempPTR).NoOfNodes == 0)
					{
						DELTempPTR = TempPTR;
						(*(*TempPTR).ParentPTR ).NoOfNodes--;
						(*(*TempPTR).ParentPTR ).NextNodePTRS[(int)(*TempPTR).Label] = NULL;

						TempPTR = (*TempPTR).ParentPTR;
						if (DELTempPTR->Payload != NULL)
							//delete DELTempPTR->Payload;
						delete DELTempPTR;
						NoOfNodes--;

						SetWidthForNewNode (TempPTR);
					}
					if ( (*TempPTR).Status == 2 )
						(*TempPTR).Status = 1; 

					return 0;
				}
				return 0;
			}
		}

		if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Label == WordArray[i] )
			TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];
		else
			return -1;
	}
	return -1;
}

// *** Reset the Radix Tree ***
template<class T> void CRadixTree<T>::ResetTree()
{
	if (RootPTR == NULL)
		return;

	for (int i=0; i<256; i++)
	{
		if ((*RootPTR).NextNodePTRS[i] != NULL)
			DeleteTree((*RootPTR).NextNodePTRS[i]);
	}
}

// ***********************************************************************************
// ************************** Private Mutator Functions ******************************	
// ***********************************************************************************
template<class T> void CRadixTree<T>::SetWidthForNewNode(RadixTreeNode<T> *NodePTR)
{
	RadixTreeNode<T> *TempPTR = NodePTR;

	while (TempPTR != NULL)
	{
		(*TempPTR).Width = CalculateWidth(TempPTR);
		TempPTR = (*TempPTR).ParentPTR;
	}
}

template<class T> RadixTreeNode<T>* CRadixTree<T>::GetMaximumNodeChild(RadixTreeNode<T> *NodePTR)
{
	RadixTreeNode<T> *TempPTR = NULL; 
	int Max = 1;

	if (NodePTR == NULL)
		return TempPTR;

	if ((*NodePTR).NoOfNodes == 0)
		return TempPTR;
	else
	{
		for (int i=0; i<256; i++)
		{
			if ((*NodePTR).NextNodePTRS[i] != NULL)
			{
				if ((*(*NodePTR).NextNodePTRS[i] ).NoOfNodes > Max)
				{
					Max = (*(*NodePTR).NextNodePTRS[i]).NoOfNodes;
					TempPTR = (*NodePTR).NextNodePTRS[i];
				}
			}
		}
		return TempPTR;
	}
	return TempPTR;
}

template<class T> int CRadixTree<T>::CalculateWidth(RadixTreeNode<T> *NodePTR)
{
	if ((*NodePTR).NoOfNodes == 0)
		return 1;
	else
	{
		int Temp = 0;
		for (int i=0; i<256; i++)
		{
			if ((*NodePTR).NextNodePTRS[i] != NULL)
				Temp = Temp + (*(*NodePTR).NextNodePTRS[i]).Width;
		}
		return Temp;
	}
	return 1;
}

template<class T> void CRadixTree<T>::DeleteTree(RadixTreeNode<T> *RootPTR1)
{
	// Base Cases.
	if (RootPTR1 == NULL)
		return;

	if ((*RootPTR1).NoOfNodes == 0)
	{
		if ((*RootPTR1).ParentPTR == NULL)
		{
			NoOfNodes--;
			delete RootPTR1;

			return;
		}
		else
		{
			(*(*RootPTR1).ParentPTR).NextNodePTRS[(int)(*RootPTR1).Label] = NULL;
			(*(*RootPTR1).ParentPTR).NoOfNodes--;
			SetWidthForNewNode((*RootPTR1).ParentPTR);

			NoOfNodes--;
			delete RootPTR1;

			return;
		}
	}
	// Else if it is not the base case.
	else
	{
		for (int i=0; i<256; i++)
		{
			if ((*RootPTR1).NextNodePTRS[i] != NULL)
				DeleteTree((*RootPTR1).NextNodePTRS[i]);
		}
		DeleteTree(RootPTR1);
		return;
	}
}
// ***********************************************************************************
// ************************ Private Mutator Functions End ****************************
// ***********************************************************************************
