#include "LZ77.h"
#include <iostream>
#include <fstream>
using std::fstream;
using std::cout;
using std::cerr;
using std::endl;

CLZ77::CLZ77(): MaxDictionarySize(256)
{
}

CLZ77::~CLZ77()
{
}

unsigned short int CLZ77::SearchDictionary (vector<char> SearchWindow)
{
	for (unsigned short int i=0; i<(unsigned short int)Dictionary.size(); i++)
	{
		if (Dictionary[i].Phrase.size() != SearchWindow.size())
			continue;
		for (unsigned short int j=0; j<(unsigned short int)Dictionary[i].Phrase.size(); j++)
		{
			if (Dictionary[i].Phrase[j] != SearchWindow[j])
				break;
			if (j == Dictionary[i].Phrase.size()-1)
				return (unsigned short int)(i+1);
		}
	}
	return 0U;
}

void CLZ77::Encode (char *InFilename, char *OutFilename)
{
	fstream InFile (InFilename, std::ios::in|std::ios::binary);
	InFile.seekg(0, std::ios::end);
	int InFileSize = (int)InFile.tellg();
	if (!InFile || InFileSize < 0)
	{
		cerr << "ERROR: Openining file: " << InFilename << endl;
		exit(-1);
	}

	cout << "Encoding " << InFilename << endl;
	cout << "InFile size: " << InFileSize << endl;
	InFile.seekg(std::ios::beg);

	fstream OutFile(OutFilename, std::ios::out|std::ios::binary);
	if (!OutFile)
	{
		cerr << "ERROR: Openining file: " << OutFilename << endl;
		exit(-1);
	}

	Dictionary.clear();
	unsigned short int SearchIndex;
	int CurrentPosition = 0;
	char Buffer;
	DictionaryEntry temp;

	while (true)
	{
		cout << "      \r" << (float)100.f*(CurrentPosition+1)/InFileSize << "%";
		if (CurrentPosition == InFileSize)
			break;

		Window.clear();

		// Read block;
		while (true)
		{
			InFile.read(&Buffer, 1);
			Window.push_back(Buffer);
			CurrentPosition++;
			if ((SearchIndex = SearchDictionary(Window)) == 0U || CurrentPosition == InFileSize)
				break;
		}

		// Process block;
		if (CurrentPosition == InFileSize)	// End of file insertion
		{
			// Matched insertion.
			temp.Reference = (unsigned short int)(Dictionary.size()+1U);
			temp.Phrase = Window;
			temp.Encoding.Reference = SearchIndex;
			temp.Encoding.Character = '\x00';
			Dictionary.push_back(temp);
			OutFile.write((char *)&temp.Encoding.Reference, sizeof(unsigned short int));
			OutFile.write((char *)&temp.Encoding.Character, sizeof(char));
			break;
		}
		else if (Window.size() == 1)	// If single character.
		{
			temp.Reference = (unsigned short int)Dictionary.size()+1U;
			temp.Phrase = Window;
			temp.Encoding.Reference = 0U;
			temp.Encoding.Character = Window[0];
			Dictionary.push_back(temp);
			OutFile.write((char *)&temp.Encoding.Reference, sizeof(unsigned short int));
			OutFile.write((char *)&temp.Encoding.Character, sizeof(char));
		}
		else // 
		{
			temp.Phrase = Window;
			temp.Encoding.Character = Window[Window.size()-1];
			Window.pop_back();
			temp.Encoding.Reference = SearchDictionary(Window);
			Dictionary.push_back(temp);
			OutFile.write((char *)&temp.Encoding.Reference, sizeof(unsigned short int));
			OutFile.write((char *)&temp.Encoding.Character, sizeof(char));
		}
	}
	cout << endl;
	InFile.close();
	OutFile.close();
}

void CLZ77::Decode (char *InFilename, char *OutFilename)
{
	fstream InFile (InFilename, std::ios::in|std::ios::binary);
	InFile.seekg(0, std::ios::end);
	int InFileSize = (int)InFile.tellg();
	if (!InFile || InFileSize < 0)
	{
		cerr << "ERROR: Openining file: " << InFilename << endl;
		exit(-1);
	}

	cout << "Decoding " << InFilename << endl;
	cout << "InFile size: " << InFileSize << endl;
	InFile.seekg(std::ios::beg);

	fstream OutFile(OutFilename, std::ios::out|std::ios::binary);
	if (!OutFile)
	{
		cerr << "ERROR: Openining file: " << OutFilename << endl;
		exit(-1);
	}

	Dictionary.clear();
	int CurrentPosition = 0;
	unsigned short int ShortBuffer;
	char Buffer;
	DictionaryEntry temp;

	while (true)
	{
		cout << "                                        \r" << (float)100.f*CurrentPosition/InFileSize << "%: Dictionary size  = " << Dictionary.size();
		if (CurrentPosition >= InFileSize || InFile.eof())
			break;

		Window.clear();
		InFile.read((char *)&ShortBuffer, sizeof(unsigned short int));
		InFile.read((char *)&Buffer, sizeof(char));
		CurrentPosition+=3;

		if (ShortBuffer == 0U)
		{
			Window.push_back(Buffer);
			temp.Reference = (unsigned short int)(Dictionary.size()+1U);
			temp.Phrase = Window;
			temp.Encoding.Reference = ShortBuffer;
			temp.Encoding.Character = Buffer;
			Dictionary.push_back(temp);
			OutFile.write((char *)&Window[0], sizeof(char));
		}
		else
		{
			Window = Dictionary[ShortBuffer-1U].Phrase;
			if (Buffer != '\x00')
				Window.push_back(Buffer);
			temp.Reference = (unsigned short int)(Dictionary.size()+1U);
			temp.Phrase = Window;
			temp.Encoding.Character = Buffer;
			temp.Encoding.Reference = ShortBuffer;
			Dictionary.push_back(temp);
			for (int i=0; i<(int)Window.size(); i++)
				OutFile.write((char *)&Window[i], sizeof(char));
		}
	}
	InFile.close();
	OutFile.close();
}
