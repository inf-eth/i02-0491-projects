#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

struct EncodingPair
{
	short int Reference;
	char Character;
};

struct DictionaryEntry
{
	short int Reference;
	vector<char> Phrase;
	EncodingPair Encoding;
};

class CLZ77
{
	vector<char> Window;
	//unsigned int WindowSize;
	vector<DictionaryEntry> Dictionary;
	int SearchDictionary (vector<char>);

public:
	CLZ77();
	~CLZ77();

	void Encode (char *, char *);
	void Decode (char *, char *);
};

