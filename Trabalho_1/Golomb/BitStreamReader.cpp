//
// Created by root on 05-10-2015.
//

#include <cmath>
#include "BitStreamReader.h"

using namespace std;

BitStreamReader::BitStreamReader(std::string filePath) {
    fileRead.open(filePath, ios::in|ios::binary|ios::ate);
    fileRead.seekg(0, ios::end);
    fileSize = fileRead.tellg();
    bufferSize = getBufferSize();
    fileReadPosition = 0;
    bufferReadBitPosition = 7;
    bufferReadBytePosition = 0;
    readBuffer = new char[bufferSize];
    refreshBufferContents();
}

int BitStreamReader::readBit() {
    /* we already have data on the buffer, lets parse it */
    if(fileReadPosition == fileSize + bufferSize){
        fileRead.close();
        return 37;  // error 37, reached end of file
    }

    char value = readBuffer[bufferReadBytePosition];
    int returnValue = (value >> bufferReadBitPosition) & 1;
    bufferReadBitPosition--;
    if(bufferReadBitPosition < 0){
        bufferReadBitPosition = 7;
        bufferReadBytePosition++;
    }
    if(bufferReadBytePosition >= bufferSize){
        bufferReadBytePosition = 0;
        bufferReadBitPosition = 7;
        refreshBufferContents();
    }

    return returnValue;
}

unsigned int BitStreamReader::getBufferSize() {
    if(fileSize == 1)
        return 1;
    unsigned int i;
    for(i = 2; i < sqrt(fileSize); i++){
        if((fileSize % i) == 0){
            break;
        }
    }
    return fileSize/i;
}

void BitStreamReader::refreshBufferContents() {
    if(fileReadPosition <= fileSize) {
        fileRead.seekg(fileReadPosition, ios::beg);
        fileRead.read(readBuffer, bufferSize);
        fileReadPosition += bufferSize;
    }
}
