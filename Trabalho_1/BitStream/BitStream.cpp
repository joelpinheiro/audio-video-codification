#include "BitStream.h"
#include <math.h>

using namespace std;

BitStream::BitStream(string fn, unsigned int bf)
{
    beginStream = 0;
    endStream = 0;
    bufferSize = bf;
    fileName = fn;
    allocBuffer();
}

void BitStream::openFileToRead(){
    fileHandleRead.open(fileName, ios::in|ios::binary|ios::ate);
    fileHandleRead.seekg(ios::beg);
    beginStream = fileHandleRead.tellg();
    fileHandleRead.seekg(0, ios::end);
    endStream = fileHandleRead.tellg();
    fileHandleRead.seekg(ios::beg);
}

void BitStream::closeFileRead(){
    fileHandleRead.close();
}

void BitStream::closeFileWrite(){
    fileHandleWrite.close();
}

unsigned int BitStream::getSize(){
    return (unsigned int) (endStream - beginStream);
}

string BitStream::getFileName(){
    return fileName;
}

void BitStream::readBit(int pos){
    fileHandleRead.seekg(pos, ios::beg);
    fileHandleRead.read(memBlock, bufferSize);
}

int BitStream::getBitToText(unsigned int byte, unsigned int position){
    return ((memBlock[byte] >> position) & 1);
}

char BitStream::getBitToBin(unsigned int byte) {
    return memBlock[byte];
}

void BitStream::openFileToWrite(int flag){
    openFileToRead();
    closeFileRead();
    if(flag == CREATE_AND_REPLACE) {     // create if it doesnt exist
        fileHandleWrite.open(fileName, ios::out | ios::binary | ios::in | ios::trunc);
    }
    else if(flag == REPLACE_ONLY){     // does not create, replace only
        fileHandleWrite.open(fileName, ios::out | ios::binary | ios::in);
    }
    else{
        fileHandleWrite.open(fileName, ios::out | ios::binary | ios::in);
    }
}

void BitStream::writeBit(int pos, int byte){
    fileHandleWrite.seekp(pos, ios::beg);                 // pointer to start writing
    fileHandleWrite.write(memBlock, byte);
}

char* BitStream::getBufferContents(){
    return memBlock;
}

void BitStream::setBufferContents(char* newBuffer) {
    memBlock = newBuffer;
}

void BitStream::setBufferSize(unsigned int buffSize) {
    bufferSize = buffSize;
    allocBuffer();
}

void BitStream::allocBuffer() {
    memBlock = new char[bufferSize];
}

unsigned int BitStream::getBufferSize() {
    return bufferSize;
}

unsigned int BitStream::getOptimalBufferSize() {
    unsigned int i, fileSize = getSize();

    if(fileSize == 1)
        return 1;

    for(i = 2; i < sqrt(fileSize); i++){
        if((fileSize % i) == 0){
            break;
        }
    }
    return fileSize/i;
}