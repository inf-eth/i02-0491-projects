#include "OpenSSLCli.h"

int main (int argc, char **argv)
{
	char choice;
	cout << "1. Compute Hash." << endl;
	cout << "2. Sign a file." << endl;
	cout << "3. Symmetric Encryption." << endl;
	cout << "4. Symmetric Decryption." << endl;
	cout << "Enter a choice: ";
	cin >> choice;
	
	switch (choice)
	{
		case '1':
		{
			string hashtype, hashfileout, filein;
			cout << "Enter hash type (e.g. sha1): ";
			cin >> hashtype;
			cout << "Enter output hash file ('.' for stdout): ";
			cin >> hashfileout;
			cout << "Enter input file: ";
			cin >> filein;
			
			OpenSSL_dgst (hashtype, hashfileout, filein);
			break;
		}
		default:
		{
			break;
		}
		
	}
	return 0;
}
