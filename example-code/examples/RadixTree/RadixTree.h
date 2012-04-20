// RadixTreeDictionary.h: interface for the CRadixTreeDictionary class.
//
//////////////////////////////////////////////////////////////////////

// The Standard Radix Tree Node.
struct RadixTreeDictionaryNode
{
	char Label;
	char RedFlag;
	short  NoOfNodes;
	short  Status;
	short  Width;
	CString Meaning;

	// Pointers.
	RadixTreeDictionaryNode *ParentPTR;
	RadixTreeDictionaryNode *NextNodePTRS[256];
};

// *** The Radix Tree Dictionary Class Definition ***
class CRadixTreeDictionary  
{
public:
	RadixTreeDictionaryNode *RootPTR;

	// *** Constructor/Destructor ***
	CRadixTreeDictionary();
	virtual ~CRadixTreeDictionary();

	// ************************ Public Functions **************************
	void CreateTree ( );
	void ResetTree ( );
	
	// *** Dictionary Functions ***
	int Insert ( const CString&, const CString& );
	int Search ( const CString&, DictionaryEntry&, int& );
	int Delete ( const CString& );
	// ****************************

	void ResetShashkahFlags ( const CString&, CString& );
	// ********************************************************************
private:
	int NoOfNodes;

	RadixTreeDictionaryNode *ShashkahPTR;

	// *** Private Mutator ( Whatever ) Functions ***
	void SetWidthForNewNode ( RadixTreeDictionaryNode* );
	RadixTreeDictionaryNode*  GetMaximumNodeChild ( RadixTreeDictionaryNode* );
	int  CalculateWidth ( RadixTreeDictionaryNode* );

	void DeleteTree ( RadixTreeDictionaryNode* );
	void DeleteNode ( RadixTreeDictionaryNode* );

	void ResetShashkahPTR ( );	
	// **********************************************
	/*int Width;*/	// Maybe I will need this. Maybe not.
	/*int Height*/	// Same as above.
};
