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
