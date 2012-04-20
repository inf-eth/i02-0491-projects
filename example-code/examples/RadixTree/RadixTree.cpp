// RadixTreeDictionary.cpp: implementation of the CRadixTreeDictionary class.
//
//////////////////////////////////////////////////////////////////////
#include <RadixTree.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadixTreeDictionary::CRadixTreeDictionary()
{
	RootPTR = NULL;
	ShashkahPTR = NULL;
	NoOfNodes = NULL;
}

CRadixTreeDictionary::~CRadixTreeDictionary()
{
	DeleteTree ( RootPTR );
	RootPTR = NULL;
	NoOfNodes = 0;
}

// *********************************************************************************
// ****************** Radix Tree Dictionary's Function Implementation **************
// *********************************************************************************

void CRadixTreeDictionary::CreateTree ( )
{
	RootPTR = new RadixTreeDictionaryNode;

	(*RootPTR).Label = 'R';
	(*RootPTR).NoOfNodes = 0;
	(*RootPTR).Status = -1;
	(*RootPTR).Meaning = _T("");
	(*RootPTR).ParentPTR = NULL;
	(*RootPTR).Width = 1;
	(*RootPTR).RedFlag = '0';

	for ( int i = 0; i < 256; i++ )
		(*RootPTR).NextNodePTRS[i] = NULL;

	ShashkahPTR = RootPTR;
	NoOfNodes = 0;
}

// *** Insertion in the Radix Tree ***
int CRadixTreeDictionary::Insert ( const CString &Word, const CString& Meaning )
{

	CString LocalWord = Word;
	int Length = LocalWord.GetLength ( );
	char *WordArray;

	WordArray = LocalWord.GetBuffer ( 256 );


	RadixTreeDictionaryNode *TempPTR = RootPTR;

	for ( int i = 0; i < Length; i++ )
	{

		if ( i == (Length - 1) )
		{
			
			if (  (*TempPTR).NextNodePTRS[ (int)WordArray[i] ] == NULL )
			{
				
				RadixTreeDictionaryNode *NewNode = new RadixTreeDictionaryNode;
				
				(*NewNode).Label = WordArray[i];

				(*NewNode).NoOfNodes = 0;
				(*NewNode).RedFlag = '0';
				(*NewNode).Status = 1;
				(*NewNode).Meaning = Meaning;
				(*NewNode).ParentPTR = TempPTR;
				(*TempPTR).NoOfNodes++;
				
				for ( int j = 0; j < 256; j++ )
				{
					
					(*NewNode).NextNodePTRS[j] = NULL;

				}

				(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] = NewNode;

				SetWidthForNewNode ( NewNode );

				NoOfNodes++;
				
				if ( ( *(*NewNode).ParentPTR).Status == 1 )
				{

					( *(*NewNode).ParentPTR).Status = 2;

				}
				

				return 1;

			}
			
			else
			{

				( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Meaning = Meaning;
				
				if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 0 )
				{
					
					( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status = 2;

				}

				return 1;

			}

		}

		else
		{

			if ( (*TempPTR).NextNodePTRS[ (int)WordArray[i] ] == NULL )
			{

				RadixTreeDictionaryNode *NewNode = new RadixTreeDictionaryNode;
				
				(*NewNode).Label = WordArray[i];

				(*NewNode).NoOfNodes = 0;
				(*NewNode).RedFlag = '0';
				(*NewNode).Status = 0;
				(*NewNode).Meaning = _T("");
				(*NewNode).ParentPTR = TempPTR;
				(*TempPTR).NoOfNodes++;
				
				for ( int k = 0; k < 256; k++ )
				{
					
					(*NewNode).NextNodePTRS[k] = NULL;

				}

				(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] = NewNode;

				SetWidthForNewNode ( NewNode );

				NoOfNodes++;
				
				TempPTR = (*TempPTR).NextNodePTRS[ (int)WordArray[i] ];

				if ( ( *(*NewNode).ParentPTR).Status == 1 )
				{

					( *(*NewNode).ParentPTR).Status = 2;

				}

			}

			else
			{
			
				TempPTR = (*TempPTR).NextNodePTRS[ (int)WordArray[i] ];

			}
			
		}

		


	}

	return 0;

}

// *** Search in the Radix Tree ***

int CRadixTreeDictionary::Search ( const CString &Word, DictionaryEntry &Temp, int &ComparisonCounter )
{

	CString LocalWord = Word;
	int Length = LocalWord.GetLength ( );
	char *WordArray;

	WordArray = LocalWord.GetBuffer ( 256 );


	RadixTreeDictionaryNode *TempPTR = RootPTR;

	for ( int i = 0; i < Length; i++ )
	{

		ComparisonCounter++;

		if ( (*TempPTR).NextNodePTRS[ (int)WordArray[i] ] == NULL )
		{

			return 0;

		}

		if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label != WordArray[i] )
		{

			return 0;

		}

		if ( i == Length - 1 )
		{

			if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 1 || ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 2 )
			{

				Temp.Word = LocalWord;
				Temp.Meaning = ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Meaning;

				return 1;

			}

		}

		if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label == WordArray[i] )
		{

			TempPTR = (*TempPTR).NextNodePTRS[ (int)WordArray[i] ];

		}

		else
		{

			return 0;
		}

	}
	
	return 0;

}

// *** Deletion in the Radix Tree ***

int CRadixTreeDictionary::Delete ( const CString &Word )
{

	CString LocalWord = Word;
	int Length = LocalWord.GetLength ( );
	char *WordArray;

	WordArray = LocalWord.GetBuffer ( 256 );

	RadixTreeDictionaryNode *TempPTR = RootPTR;

	for ( int i = 0; i < Length; i++ )
	{

		if ( (*TempPTR).NextNodePTRS[ (int)WordArray[i] ] == NULL )
		{

			return 0;

		}

		if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label != WordArray[i] )
		{

			return 0;

		}

		if ( i == Length - 1 )
		{

			if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 1 || ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 2 )
			{

				/*
				Call the delete this node function
				*/

				RadixTreeDictionaryNode *DELTempPTR;
				TempPTR = (*TempPTR).NextNodePTRS[ (int)WordArray[i] ];

				if ( (*TempPTR).Status == 2 )
				{

					(*TempPTR).Meaning = _T("");
					(*TempPTR).Status = 0;
					
					return 1;

				}

				else if ( (*TempPTR).Status == 1 )
				{

					while ( TempPTR != RootPTR && (*TempPTR).Status != 2 && (*TempPTR).NoOfNodes == 0 )
					{
						
						DELTempPTR = TempPTR;
						( *(*TempPTR).ParentPTR ).NoOfNodes--;
						( *(*TempPTR).ParentPTR ).NextNodePTRS[ (int)(*TempPTR).Label ] = NULL;

						TempPTR = (*TempPTR).ParentPTR;
						
						delete DELTempPTR;
						NoOfNodes--;

						SetWidthForNewNode ( TempPTR );
						

					}

					if ( (*TempPTR).Status == 2 )
					{

						(*TempPTR).Status = 1; 

					}

					return 1;

				}

				return 1;

			}

		}

		if ( ( *(*TempPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label == WordArray[i] )
		{

			TempPTR = (*TempPTR).NextNodePTRS[ (int)WordArray[i] ];

		}

		else
		{

			return 0;
		}

	}
	
	return 0;
}

// *** Reset the Radix Tree ***

void CRadixTreeDictionary::ResetTree ( )
{

	if ( RootPTR == NULL )
	{

		return;

	}

	for ( int i = 0; i < 256; i++ )
	{
		if ( (*RootPTR).NextNodePTRS[i] != NULL )
		{

			DeleteTree ( (*RootPTR).NextNodePTRS[i] );

		}

	}

}
// ***********************************************************************************
// ************************** Private Mutator Functions ******************************	
// ***********************************************************************************

void CRadixTreeDictionary::SetWidthForNewNode ( RadixTreeDictionaryNode *NodePTR )
{

	RadixTreeDictionaryNode *TempPTR = NodePTR;
	//RadixTreeDictionaryNode *XTempPTR;
	
	while ( TempPTR != NULL )
	{

		//XTempPTR = TempPTR;
		(*TempPTR).Width = CalculateWidth ( TempPTR );
		TempPTR = (*TempPTR).ParentPTR;

	}

}


RadixTreeDictionaryNode* CRadixTreeDictionary::GetMaximumNodeChild ( RadixTreeDictionaryNode *NodePTR )
{

	RadixTreeDictionaryNode *TempPTR = NULL; 
	int Max = 1;

	if ( NodePTR == NULL )
	{

		return TempPTR;

	}

	if ( (*NodePTR).NoOfNodes == 0 )
	{

		return TempPTR;

	}

	else
	{

		for ( int i = 0; i < 256; i++ )
		{

			if ( (*NodePTR).NextNodePTRS[i] != NULL )
			{

				if ( ( *(*NodePTR).NextNodePTRS[i] ).NoOfNodes > Max )
				{
					
					Max = ( *(*NodePTR).NextNodePTRS[i] ).NoOfNodes;
					TempPTR = (*NodePTR).NextNodePTRS[i];

				}

			}

		}

		return TempPTR;

	}

	return TempPTR;

}

int CRadixTreeDictionary::CalculateWidth ( RadixTreeDictionaryNode* NodePTR )
{

	if ( (*NodePTR).NoOfNodes == 0 )
	{
		
		return 1;

	}

	else
	{

		int Temp = 0;
		
		for ( int i = 0; i < 256; i++ )
		{

			if ( (*NodePTR).NextNodePTRS[i] != NULL )
			{

				Temp = Temp + ( *(*NodePTR).NextNodePTRS[i] ).Width;

			}

		}
		
		return Temp;		

	}

	return 1;

}

void CRadixTreeDictionary::DeleteTree ( RadixTreeDictionaryNode *RootPTR1 )
{

	// Base Cases.

	if ( RootPTR1 == NULL )
	{

		return;

	}

	if ( (*RootPTR1).NoOfNodes == 0 )
	{

		if ( (*RootPTR1).ParentPTR == NULL )
		{

			NoOfNodes--;

			if ( ShashkahPTR == RootPTR1 )
			{
				ResetShashkahPTR ( );
			}

			delete RootPTR1;
			return;

		}

		else
		{

			( *(*RootPTR1).ParentPTR ).NextNodePTRS[ (int)(*RootPTR1).Label ] = NULL;
			( *(*RootPTR1).ParentPTR ).NoOfNodes--;
			SetWidthForNewNode ( (*RootPTR1).ParentPTR );

			NoOfNodes--;

			if ( ShashkahPTR == RootPTR1 )
			{
				ResetShashkahPTR ( );
			}

			delete RootPTR1;
			return;

		}

	}

	// Else if it is not the base case.
	else
	{

		for ( int i = 0; i < 256; i++ )
		{

			if ( (*RootPTR1).NextNodePTRS[i] != NULL )
			{

				DeleteTree ( (*RootPTR1).NextNodePTRS[i] );

			}

		}

		DeleteTree ( RootPTR1 );
		
		return;

	}

}






// ***********************************************************************************
// ************************ Private Mutator Functions End ****************************	
// ***********************************************************************************

void CRadixTreeDictionary::ResetShashkahFlags ( const CString &SearchString, CString &Meaning )
{

	ResetShashkahPTR ( );

	CString LocalWord = SearchString;
	int Length = LocalWord.GetLength ( );
	char *WordArray;

	WordArray = LocalWord.GetBuffer ( 256 );

	RadixTreeDictionaryNode *TempPTR;

	for ( int i = 0; i < Length; i++ )
	{

		if ( (*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] == NULL )
		{

			ResetShashkahPTR ( );
			return;

		}

		if ( ( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label != WordArray[i] )
		{

			ResetShashkahPTR ( );
			return;

		}

		if ( i == Length - 1 )
		{

			if ( ( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 1 || ( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).Status == 2 )
			{

				Meaning = ( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).Meaning;

				( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).RedFlag = 'R';
				
				TempPTR = ShashkahPTR;

				ShashkahPTR = (*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ]; 

				while ( TempPTR != RootPTR  )
				{

					(*TempPTR).RedFlag = 'R';
					TempPTR = (*TempPTR).ParentPTR;

				}
				return;

			}

		}

		if ( ( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).Label == WordArray[i] )
		{

			( *(*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ] ).RedFlag = 'G';
			ShashkahPTR = (*ShashkahPTR).NextNodePTRS[ (int)WordArray[i] ];

		}

		else
		{

			ResetShashkahPTR ( );
			return;

		}

	}
	

}

void CRadixTreeDictionary::ResetShashkahPTR ( )
{
	
	while ( ShashkahPTR != RootPTR )
	{

		(*ShashkahPTR).RedFlag = '0';
		ShashkahPTR = (*ShashkahPTR).ParentPTR;

	}


}
