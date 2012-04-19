#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

struct EncodingPair
{
	unsigned short int Reference;
	char Character;
};

struct DictionaryEntry
{
	unsigned short int Reference;
	vector<char> Phrase;
	EncodingPair Encoding;
};

class CLZ77
{
	int MaxDictionarySize;
	vector<char> Window;
	vector<DictionaryEntry> Dictionary;
	unsigned short int SearchDictionary (vector<char>);

public:
	CLZ77();
	~CLZ77();

	void Encode (char *, char *);
	void Decode (char *, char *);
};

