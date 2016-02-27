#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <string>

using namespace std;

/*
 *	BitStream: "name" is the name of the file to read from. "mode" is "r"
 *	for reading and "w" for writing. If file opening fails, "exit" is called.
 *
 *	ReadBit: returns 0 or 1.
 *
 *	ReadNBits: returns an integer with the "n" least significant bits read
 *	from the stream. The least significant bit is the last one read.
 *
 *	ReadString: reads groups of 8 bits (interpreted as ASCII codes) until
 *	reading a newline ('\n'). The newline is not returned.
 *
 *	WriteBit: writes the least significant bit of the integer "bit".
 *
 *	WriteNBits: writes the "n" least significant bits from integer "bits".
 *	The least significant bit is the last one to be written.
 *
 *	WriteString: writes groups of 8 bits (interpreted as ASCII codes) until
 *	the end of the string, followed by a newline ('\n').
 *
 *	Close: closes the stream. In the case of a stream opened for writing,
 *	the internal buffer is flushed, if necessary.
 *
 *	ReadMode: returns "true" if the stream was opened for reading, "false"
 *	otherwise.
 */

class BitStream {
	public:
		BitStream(const char* name, const char* mode);
		int ReadBit(void);
		int ReadNBits(int n);
		string ReadString(void);
		void WriteBit(int bit);
		void WriteNBits(int bits, int n);
		void WriteString(const string& s);
		void Close(void);
		bool ReadMode(void);
	private:
		fstream file;
		bool reading;
		int buffer;
		int ptr;
};

#endif

