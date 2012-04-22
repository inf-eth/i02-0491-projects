#include <rabinhash32.h>


RabinHashFunction32::RabinHashFunction32(const int vP):P(vP)
{
	initializeTables();
}
RabinHashFunction32::~RabinHashFunction32(){
	delete[] table32;
	delete[] table40;
	delete[] table48;
	delete[] table56;
}
void RabinHashFunction32::initializeTables() {

	int*  mods = new int[P_DEGREE];
	// We want to have mods[i] == x^(P_DEGREE+i)
	mods[0] = P;
	for (int i = 1; i < P_DEGREE; i++) {
		const int lastMod = mods[i - 1];
		// x^i == x(x^(i-1)) (mod P)
		int thisMod = lastMod << 1;
		// if x^(i-1) had a x_(P_DEGREE-1) term then x^i has a
		// x^P_DEGREE term that 'fell off' the top end.
		// Since x^P_DEGREE == P (mod P), we should add P
		// to account for this:
		if ((lastMod & X_P_DEGREE) != 0) {
			thisMod ^= P;
		}
		mods[i] = thisMod;

	}
	// Let i be a number between 0 and 255 (i.e. a byte).
	// Let its bits be b0, b1, ..., b7.
	// Let Q32 be the polynomial b0*x^39 + b1*x^38 + ... + b7*x^32 (mod P).
	// Then table32[i] is Q32, represented as an int (see below).
	// Likewise Q40 be the polynomial b0*x^47 + b1*x^46 + ... + b7*x^40 (mod P).
	// table40[i] is Q40, represented as an int. Likewise table48 and table56.

	table32 = new int[256];
	table40 = new int[256];
	table48 = new int[256];
	table56 = new int[256];

	for (int i = 0; i < 256; i++) {
		int c = i;
		for (int j = 0; j < 8 && c > 0; j++) {
			if ((c & 1) != 0) {
				table32[i] ^= mods[j];
				table40[i] ^= mods[j + 8];
				table48[i] ^= mods[j + 16];
				table56[i] ^= mods[j + 24];
			}
			c >>= 1;
		}
	}
	delete[] mods;
}

int RabinHashFunction32::getP() {
	return P;
}

int RabinHashFunction32::computeWShifted(const int w){

	return table32[w & 0xFF] ^table40[(w >> 8) & 0xFF] ^table48[(w >> 16) & 0xFF] ^ table56[(w >> 24) & 0xFF];

}

int RabinHashFunction32::hash(const char A[],const int size) {
    return hash(A, 0, size, 0);
}
int RabinHashFunction32::hash(const char A[], const int offset, const int length, int w) {

    int s = offset;

    // First, process a few bytes so that the number of bytes remaining is a multiple of 4.
    // This makes the later loop easier.
    const int starterBytes = length % 4;
    if (starterBytes != 0) {
        const int max = offset + starterBytes;
        while (s < max) {
            w = (w << 8) ^ (A[s] & 0xFF);
            s++;
        }
    }

    const int max = offset + length;
    while (s < max) {
        w = computeWShifted(w) ^
            (A[s] << 24) ^
            ((A[s + 1] & 0xFF) << 16) ^
            ((A[s + 2] & 0xFF) << 8) ^
            (A[s + 3] & 0xFF);
        s += 4;
    }

    return w;
}
