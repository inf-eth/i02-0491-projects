/*
 * rabinhash-c++.h
 *
 *  Created on: Mar 31, 2009
 *      Author: luo
 *      由Sean Owen写的java版本改编而来，算法思想是:
 *
 * <p>This class provides an implementation of a hash function based on Rabin fingerprints, one
 * which can efficiently produce a 32-bit hash value for a sequence of bytes. It does so by considering
 * strings of bytes as large polynomials over GF(2) -- that is, with coefficients of 0 and 1 --
 * and then reducing them modulo some irreducible polynomial of degree 32. The result is a hash function
 * with very satisfactory properties. In addition the polynomial operations are fast in hardware;
 * even in this Java implementation the speed is reasonable.</p>
 *
 * <p>Methods in this class can compute a hash value for an array of bytes, chars or ints, as well as
 * any {@link java.io.Serializable} object, String, file, or resource denoted by URL.</p>
 *
 * <p>Methods of this class are all thread-safe, and hash function objects are immutable.</p>
 *
 * <p>Polynomials of degree 32 are used frequently in this code, and are represented efficiently as
 * <code>int</code>s. An <code>int</code> has 32 bits, whereas a polynomial of degree 32 has 33 coefficients.
 * Therefore, the high-order bit of the <code>int</code> is the degree 31 term's
 * coefficient, and the low-order bit is the constant coefficient.</p>
 *
 * <p>For example the integer 0x00000803, in binary, is:</p>
 *
 * <p><code>00000000 00000000 00001000 00000011</code></p>
 *
 * <p>Therefore it correponds to the polynomial:</p>
 *
 * <p><code>x<sup>32</sup> + x<sup>11</sup> + x + 1</code></p>
 *
 * <p>The implementation is derived from the paper "Some applications of Rabin's fingerprinting method"
 * by Andrei Broder. See <a href="http://server3.pa-x.dec.com/SRC/publications/src-papers.html">
 * http://server3.pa-x.dec.com/SRC/publications/src-papers.html</a> for a full citation and the paper
 * in PDF format.</p>
 *
 *
 */

#ifndef RABINHASHC_H_
#define RABINHASHC_H_

class RabinHashFunction32 {
private:
	/** Represents x<sup>32</sup> + x<sup>7</sup> + x<sup>3</sup> + x<sup>2</sup> + 1. */
	static const int DEFAULT_IRREDUCIBLE_POLY = 0x0000008D;

	/** Default hash function, provided for convenience. */
	//static const RabinHashFunction32 DEFAULT_HASH_FUNCTION =
			//RabinHashFunction32(DEFAULT_IRREDUCIBLE_POLY);

	static const int P_DEGREE = 32;
	static const int X_P_DEGREE = 1 << (P_DEGREE - 1);
	static const int READ_BUFFER_SIZE = 1024;

	const int P;
	int *table32, *table40, *table48, *table56;
	void initializeTables();
public:
	/**
	 * <p>Creates a RabinHashFunction32 based on the specified polynomial.</p>
	 *
	 * <p>This class does not test the polynomial for irreducibility; therefore this constructor should
	 * only be used with polynomials that are already known to be irreducible, or else the hash function
	 * will not perform optimally.</p>
	 *
	 * @param P a degree 32 polynomial over GF(2), represented as an <code>int</code>
	 */

	RabinHashFunction32(const int P);
	~RabinHashFunction32();
	/**
	 * @return irreducible polynomial used in this hash function, represented as an <code>int</code>
	 */
	int getP();

	int computeWShifted(const int w);

	/**
	 * <p>Return the Rabin hash value of an array of bytes.</p>
	 *
	 * @param A the array of bytes
	 * @return the hash value
	 * @throws NullPointerException if A is null
	 */
	int hash(const char A[], const int size);

	int hash(const char A[], const int offset, const int length, int w);

};
#endif /* RABINHASHC_H_ */
