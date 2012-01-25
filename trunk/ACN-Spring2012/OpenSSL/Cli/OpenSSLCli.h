#include <string>
#include <fstream>
#include <iostream>
using std::string;
using std::cout;
using std::cin;
using std::endl;

/* Compute message digest.
First argument is hash type, e.g. sha1.
Second argument is output file, an empty filename will output to stdout.
Third argument is input file whose hash is to be computed.
*/
int OpenSSL_dgst (string, string, string);

/* Sign message digest.
Frist argument is hash type, e.g. sha1 for RSA and dss1 for DSA.
Second argument is the filename containing private key.
Third argument is the output filename which will contain the signature.
Fourth argument is the input file whose hash is to be computed and signed.
*/
int OpenSSL_dgst_sign (string, string, string, string);
