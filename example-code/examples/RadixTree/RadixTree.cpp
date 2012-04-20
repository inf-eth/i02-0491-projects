// RadixTree.cpp: implementation of the CRadixTree class.
//
//////////////////////////////////////////////////////////////////////

#include <RadixTree.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadixTree::CRadixTree()
{
	RootPTR = NULL;
	NoOfNodes = NULL;
}

CRadixTree::~CRadixTree()
{
	DeleteTree(RootPTR);
	RootPTR = NULL;
	NoOfNodes = 0;
}

// *********************************************************************************
// ****************** Radix Tree Dictionary's Function Implementation **************
// *********************************************************************************

void CRadixTree::CreateTree()
{
	RootPTR = new RadixTreeNode;

	(*RootPTR).Label = 'R';
	(*RootPTR).NoOfNodes = 0;
	(*RootPTR).Status = -1;
	(*RootPTR).Payload = NULL;
	(*RootPTR).ParentPTR = NULL;
	(*RootPTR).Width = 1;

	for (int i=0; i<256; i++)
		(*RootPTR).NextNodePTRS[i] = NULL;

	NoOfNodes = 0;
}

// *** Insertion in the Radix Tree ***
int CRadixTree::Insert(const char *WordArray, int Length, void* Payload)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength ( );
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer ( 256 );
	RadixTreeNode *TempPTR = RootPTR;

	for (int i=0; i<Length; i++)
	{
		if (i == (Length-1))
		{
			if ((*TempPTR).NextNodePTRS[(int)WordArray[i]] == NULL)
			{
				RadixTreeNode *NewNode = new RadixTreeNode;
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
				RadixTreeNode *NewNode = new RadixTreeNode;
				(*NewNode).Label = WordArray[i];

				(*NewNode).NoOfNodes = 0;
				(*NewNode).Status = 0;
				(*NewNode).Payload = NULL;
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
void* CRadixTree::Search(const char* WordArray, int Length, int &ComparisonCounter)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength();
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer(256);
	RadixTreeNode *TempPTR = RootPTR;
	void *Temp = NULL;

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
				//Temp.Word = LocalWord;
				return (*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Payload;
		}

		if ((*(*TempPTR).NextNodePTRS[(int)WordArray[i]]).Label == WordArray[i])
			TempPTR = (*TempPTR).NextNodePTRS[(int)WordArray[i]];
		else
			return NULL;
	}
	return NULL;
}

// *** Deletion in the Radix Tree ***
int CRadixTree::Delete(const char* WordArray, int Length)
{
	//CString LocalWord = Word;
	//int Length = LocalWord.GetLength ( );
	//char *WordArray;
	//WordArray = LocalWord.GetBuffer ( 256 );
	RadixTreeNode *TempPTR = RootPTR;

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
				RadixTreeNode *DELTempPTR;
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
void CRadixTree::ResetTree()
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
void CRadixTree::SetWidthForNewNode(RadixTreeNode *NodePTR)
{
	RadixTreeNode *TempPTR = NodePTR;

	while (TempPTR != NULL)
	{
		(*TempPTR).Width = CalculateWidth(TempPTR);
		TempPTR = (*TempPTR).ParentPTR;
	}
}

RadixTreeNode* CRadixTree::GetMaximumNodeChild(RadixTreeNode *NodePTR)
{
	RadixTreeNode *TempPTR = NULL; 
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

int CRadixTree::CalculateWidth(RadixTreeNode* NodePTR)
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

void CRadixTree::DeleteTree(RadixTreeNode *RootPTR1)
{
	// Base Cases.
	if ( RootPTR1 == NULL )
		return;

	if ((*RootPTR1).NoOfNodes == 0)
	{
		if ((*RootPTR1).ParentPTR == NULL)
		{
			NoOfNodes--;
			if ((*RootPTR1).Payload != NULL)
				//delete (*RootPTR1).Payload;
			delete RootPTR1;

			return;
		}
		else
		{
			(*(*RootPTR1).ParentPTR).NextNodePTRS[(int)(*RootPTR1).Label] = NULL;
			(*(*RootPTR1).ParentPTR).NoOfNodes--;
			SetWidthForNewNode((*RootPTR1).ParentPTR);

			NoOfNodes--;
			if ((*RootPTR1).Payload != NULL)
				//delete (*RootPTR1).Payload;
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
