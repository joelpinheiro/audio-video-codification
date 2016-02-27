#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.h"

using namespace std;

BitStream::BitStream(const char* fileName, const char* mode) {
	if(mode[0] == 'r') {
		file.open(fileName, ios::in | ios::binary);
		reading = true; // Read mode
		ptr = -1;
	}

	else {
		file.open(fileName, ios::out | ios::binary);
		reading = false; // Write mode
		ptr = 7;
		buffer = 0;
	}

	if(!file.is_open())
	{
		cerr << "Error: opening file " << fileName << endl;
		exit(1);
	}
}

int BitStream::ReadBit(void) {
	if(--ptr < 0) {
		if((buffer = file.get()) == EOF)
			return EOF;

	ptr = 7;
	}

	return (buffer & (0x01 << ptr)) >> ptr;
}

int BitStream::ReadNBits(int n) {
	int x = 0;
	for(int i = 0 ; i < n ; i++) {
		x <<= 1;
		x |= this->ReadBit();
	}

	return x;
}

string BitStream::ReadString(void) {
	int c;
	string s;

	while((c = this->ReadNBits(8)) != '\n') {
		s += c;
	}

	return s;
}

void BitStream::WriteBit(int bit) {
	if(ptr < 0) {
		file.put((char)buffer);
		ptr = 7;
		buffer = 0;
	}

	buffer |= (bit & 0x01) << ptr--;
}

void BitStream::WriteNBits(int bits, int n) {
	for(int i = n - 1 ; i >= 0 ; i--)
		this->WriteBit(((0x01 << i) & bits) >> i);
}

void BitStream::WriteString(const string& s) {
	for(const char c : s) {
		this->WriteNBits(c, 8);
	}

	this->WriteNBits('\n', 8); // Mark the end of the string with a newline
}

void BitStream::Close(void) {
	if(reading) {
		file.close();
	}

	else {
		if(ptr != 7) // Flush the buffer only if there are some bits there
			file.put((char)buffer);

		file.close();
	}
}

bool BitStream::ReadMode(void) {
	return reading;
}

