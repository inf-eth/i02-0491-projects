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

void CLZ77::Encode (char *InFilename, char *OutFilename)
{
	fstream InFile (InFilename, std::ios::in);
	if (!InFile)
		cerr << "ERROR: Openining file: " << InFilename << endl;
	InFile.seekg(0, std::ios::end);
	int InFileSize = (int)InFile.tellg();
	cout << "Encoding " << InFilename << endl;
	cout << "InFile size: " << InFileSize << endl;
	InFile.seekg(std::ios::beg);

	Dictionary.clear();
	int SearchIndex;
	int CurrentPosition = 0;
	char Buffer;
	DictionaryEntry temp;

	while (true)
	{
		cout << "      \r" << (float)100.f*CurrentPosition/InFileSize << "%";
		if (CurrentPosition == InFileSize)
			break;

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
	cout << endl;
	InFile.close();

	// Write onto file.
	fstream OutFile(OutFilename, std::ios::out|std::ios::binary);
	for (int i=0; i<(int)Dictionary.size(); i++)
	{
		//for (int j=0; j<(int)Dictionary[i].Phrase.size(); j++)
			//OutFile << Dictionary[i].Phrase[j];
		//OutFile << " " << Dictionary[i].Encoding.Reference << "," << Dictionary[i].Encoding.Character << endl;
		OutFile.write((char *)&Dictionary[i].Encoding.Reference, sizeof(short int));
		OutFile.write((char *)&Dictionary[i].Encoding.Character, sizeof(char));
	}
	OutFile.close();
}

void CLZ77::Decode (char *InFilename, char *OutFilename)
{
	fstream InFile (InFilename, std::ios::in|std::ios::binary);
	if (!InFile)
		cerr << "ERROR: Openining file: " << InFilename << endl;
	InFile.seekg(0, std::ios::end);
	int InFileSize = (int)InFile.tellg();
	cout << "Decoding " << InFilename << endl;
	cout << "InFile size: " << InFileSize << endl;
	InFile.seekg(std::ios::beg);

	fstream OutFile(OutFilename, std::ios::out);
	if (!OutFile)
		cerr << "ERROR: Openining file: " << OutFilename << endl;
	
	Dictionary.clear();
	int CurrentPosition = 0;
	short int ShortBuffer;
	char Buffer;
	DictionaryEntry temp;

	while (true)
	{
		cout << "                                        \r" << (float)100.f*CurrentPosition/InFileSize << "%: Dictionary size  = " << Dictionary.size();
		if (CurrentPosition >= InFileSize || InFile.eof())
			break;

		Window.clear();
		InFile.read((char *)&ShortBuffer, sizeof(short int));
		InFile.read((char *)&Buffer, sizeof(char));
		CurrentPosition+=3;

		if (ShortBuffer == -1)
		{
			Window.push_back(Buffer);
			temp.Reference = Dictionary.size();
			temp.Phrase = Window;
			temp.Encoding.Reference = ShortBuffer;
			temp.Encoding.Character = Buffer;
			Dictionary.push_back(temp);
			OutFile.write((char *)&Window[0], sizeof(char));
		}
		else
		{
			Window = Dictionary[ShortBuffer].Phrase;
			Window.push_back(Buffer);
			temp.Reference = Dictionary.size();
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
