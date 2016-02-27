#include <iostream>
#include <cmath>
#include "Golomb.h"

using namespace std;

GolombEncoder::GolombEncoder(const char* fName, unsigned m) :
  BitStream(fName, "w") {
	this->m = m;
	b = (int)ceil(log2(m));
	l = (0x01 << b) - m;
}

void GolombEncoder::SetParameter(unsigned m) {
	this->m = m;
	b = (int)ceil(log2(m));
	l = (0x01 << b) - m;
}

void GolombEncoder::WriteInt(unsigned x) {
	unsigned q = x / m;
	unsigned r = x % m;

	for(unsigned i = 0 ; i < q ; i++)
		WriteBit(0);

	WriteBit(1); // write the comma of the unary code

	if(r < l)
		WriteNBits(r, b - 1);

	else
		WriteNBits(r + l, b);

}

GolombDecoder::GolombDecoder(const char* fName, unsigned m) :
  BitStream(fName, "r") {
	this->m = m;
	b = (int)ceil(log2(m));
	l = (0x01 << b) - m;
}

void GolombDecoder::SetParameter(unsigned m) {
	this->m = m;
	b = (int)ceil(log2(m));
	l = (0x01 << b) - m;
}

unsigned GolombDecoder::ReadInt(void) {
	unsigned q = 0, r;

	while(ReadBit() != 1)
		q++;

	if(m == 1) // In this particular case, there is no remainder
		return (unsigned)q;

	unsigned x = ReadNBits(b - 1);

	if(x < l) {
		r = x;
	}

	else {
		x = (x << 1) | ReadBit();
		r = x - l;
	}

	return (unsigned)(q * m + r);
}

