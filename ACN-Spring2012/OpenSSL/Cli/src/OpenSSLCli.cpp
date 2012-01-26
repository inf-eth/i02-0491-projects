#include <cstdlib>
#include <OpenSSLCli.h>

/* Compute message digest.
First argument is hash type, e.g. sha1.
Second argument is output file, an empty filename will output to stdout.
Third argument is input file whose hash is to be computed.
*/
int OpenSSL_dgst (string hashtype, string hashfileout, string filein)
{
	string command;
	if (hashfileout == ".")
	{
		command = (string)"openssl" + " " + "dgst" + " " + "-" + hashtype + " " + filein;
		system (command.c_str());
	}
	else
	{
		command = (string)"openssl" + " " + hashtype + " " + "-out" + " " + hashfileout + " " + filein;
		system (command.c_str());
	}
	return 0;
}

/* Sign message digest.
Frist argument is hash type, e.g. sha1 for RSA and dss1 for DSA.
Second argument is the filename containing private key.
Third argument is the output filename which will contain the signature.
Fourth argument is the input file whose hash is to be computed and signed.
*/
int OpenSSL_dgst_sign (string hashtype, string privatekeyfile, string signaturefileout, string filein)
{

	return 0;
}

/* Symmetric Encryption.
arg1 = cipher e.g., aes256, des, des3 with modes e.g., des3-ede-ofb (DES3 operating in OFB mode).
arg2 = plaintext input file.
arg3 = ciphertext output file.
*/
int OpenSSL_enc (string cipher, string plaintextfilein, string ciphertextfileout)
{
	string command = (string)"openssl enc -" + cipher + " -salt -in " + plaintextfilein + " -out " + ciphertextfileout;
	system (command.c_str());
	return 0;
}

/* Symmetric Decryption.
arg1 = cipher e.g., aes256, des, des3.
arg2 = ciphertext input file.
arg3 = plaintext output file.
*/
int OpenSSL_dec (string cipher, string cipherextfilein, string plaintextfileout)
{
	string command = (string)"openssl enc -" + cipher + " -d -in " + cihpertextfilein + " -out " + plaintextfileout;
	system (command.c_str());
	return 0;
}
