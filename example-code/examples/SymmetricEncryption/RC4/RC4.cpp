// References:
// http://www.codealias.info/technotes/des_encryption_using_openssl_a_simple_example
// http://linux.die.net/man/3/rc4_set_key
// http://old.nabble.com/About-RC4-decryption.-td32757842.html

#include <cstring>
#include <iostream>
#include <openssl/rc4.h>

using std::cout;
using std::endl;

char* Encrypt(unsigned char *keydata, char *plaintext, int Length)
{
	char *encrypted = new char[Length+1];
	RC4_KEY key;
	RC4_set_key(&key, strlen((const char*)keydata), keydata);

	// Encryption
	RC4(&key, Length, (const unsigned char*)plaintext, (unsigned char*)encrypted);
	encrypted[Length] = '\0';

	return encrypted;
}

int main()
{
	unsigned char keydata[] = "password";
	char plaintext[]="This is a secret message.";

	int Length = strlen(plaintext);

	char *Encrypted = Encrypt(keydata, plaintext, Length);
	char *Decrypted = Encrypt(keydata, Encrypted, Length);

	cout << "Plaintext: " << plaintext << endl;

	cout << "Encrypted: ";
	for (int i=0; i<Length; i++)
		cout << Encrypted[i];
	cout << endl;

	cout << "Decrypted: " << Decrypted << endl;

	return 0;
}
