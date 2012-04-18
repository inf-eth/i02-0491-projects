#include "LZ77.h"
#include <iostream>
#include <fstream>
using std::fstream;
using std::cout;
using std::cerr;
using std::endl;

CLZ77::CLZ77()
{
}

CLZ77::~CLZ77()
{
}

void CLZ77::Encode (char *InFilename, char *OutFilename)
{
	fstream InFile (InFilename, std::ios::in);
	if (!InFile)
		cerr << "ERROR: Openining file: " << InFilename << endl;
	InFile.seekg(0, std::ios::end);
	unsigned int InFileSize = (unsigned int)InFile.tellg();
	cout << "InFile size: " << InFileSize << endl;
	InFile.seekg(std::ios::beg);

	Window.resize(1);
	int SearchIndex;
	unsigned int CurrentPosition = 0;
	char Buffer;
	DictionaryEntry temp;

	while (true)
	{
		//if (CurrentPosition == InFileSize)
			//break;
		Window.clear();

		// Read block;
		while (true)
		{
			InFile.read(&Buffer, 1);
			Window.push_back(Buffer);
			CurrentPosition++;
			if ((SearchIndex = SearchDictionary(Window)) == -1 || CurrentPosition == InFileSize)
				break;
		}

		// Process block;
		if (CurrentPosition == InFileSize)	// End of file insertion
		{
			// Matched insertion.
			temp.Reference = Dictionary.size();
			temp.Phrase = Window;
			temp.Encoding.Reference = SearchIndex;
			temp.Encoding.Character = '\x00';
			Dictionary.push_back(temp);
			break;
		}
		if (Window.size() == 1)	// If single character.
		{
			temp.Reference = Dictionary.size();
			temp.Phrase = Window;
			temp.Encoding.Reference = -1;
			temp.Encoding.Character = Window[0];
			Dictionary.push_back(temp);
		}
		else // 
		{
			temp.Phrase = Window;
			temp.Encoding.Character = Window[Window.size()-1];
			Window.pop_back();
			temp.Encoding.Reference = SearchDictionary(Window);
			Dictionary.push_back(temp);
		}
	}
	InFile.close();

	// Write onto file.
	fstream OutFile(OutFilename, std::ios::out);
	for (int i=0; i<(int)Dictionary.size(); i++)
	{
		for (int j=0; j<(int)Dictionary[i].Phrase.size(); j++)
			OutFile << Dictionary[i].Phrase[j];
		OutFile << " " << Dictionary[i].Encoding.Reference << "," << Dictionary[i].Encoding.Character << endl;
	}
	OutFile.close();
}

int CLZ77::SearchDictionary (vector<char> SearchWindow)
{
	for (int i=0; i<(int)Dictionary.size(); i++)
	{
		if (Dictionary[i].Phrase.size() != SearchWindow.size())
			continue;
		for (int j=0; j<(int)Dictionary[i].Phrase.size(); j++)
		{
			if (Dictionary[i].Phrase[j] != SearchWindow[j])
				break;
			if (j == Dictionary[i].Phrase.size()-1)
				return i;
		}
	}
	return -1;
}
