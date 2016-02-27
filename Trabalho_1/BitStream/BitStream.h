#ifndef BITSTREAM_H
#define BITSTREAM_H
#include <iostream>
#include <fstream>

class BitStream
{
public:
    BitStream(std::string, unsigned int);
    void openFileToWrite(int);
    void openFileToRead();
    void closeFileRead();
    void closeFileWrite();
    unsigned int getSize();
    std::string getFileName();
    void readBit(int);
    void writeBit(int, int);
    int getBitToText(unsigned int, unsigned int);
    char getBitToBin(unsigned int);
    char* getBufferContents();
    void setBufferContents(char*);
    void setBufferSize(unsigned int);
    unsigned int getBufferSize();
    unsigned int getOptimalBufferSize();

#define CREATE_AND_REPLACE 0
#define REPLACE_ONLY 1
protected:
private:
    void allocBuffer();
    std::streampos beginStream, endStream;
    std::ifstream fileHandleRead;
    std::ofstream fileHandleWrite;
    std::string fileName;
    char* memBlock;
    unsigned int bufferSize;
};

#endif // BITSTREAM_H
