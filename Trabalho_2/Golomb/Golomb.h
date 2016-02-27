#ifndef GOLOMB2_H
#define GOLOMB2_H

#include <cmath>
#include "BitStream.h"

using namespace std;

/*
 *	GolombEncoder: starts a Golomb encoder, using Golomb parameter "m" and
 *	writing to file "fName". It calls "exit" if fails to open the file.
 *
 *	ReadInt: returns the next (non-negative) integer from the stream.
 *
 *	WriteInt: writes a non-negative integer into the stream.
 */

class GolombEncoder: public BitStream {
	public:
		GolombEncoder(const char* fName, unsigned m = 1);
		void SetParameter(unsigned m);
		void WriteInt(unsigned x);
	private:
		unsigned m; // Golomb parameter
		unsigned b; // ceil(log2(m))
		unsigned l; // 2^b - m
};

/*
 *	GolombDecoder: starts a Golomb decoder, using Golomb parameter "m" and
 *	reading from file "fName". It calls "exit" if fails to open the file.
 *
 *	ReadInt: returns the next (non-negative) integer from the stream.
 */

class GolombDecoder: public BitStream {
	public:
		GolombDecoder(const char* fName, unsigned m = 1);
		void SetParameter(unsigned m);
		unsigned ReadInt(void);
	private:
		unsigned m; // Golomb parameter
		unsigned b; // ceil(log2(m))
		unsigned l; // 2^b - m
};

#endif

