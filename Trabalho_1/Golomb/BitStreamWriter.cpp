//
// Created by root on 05-10-2015.
//

#include <cmath>
#include "BitStreamWriter.h"

using namespace std;

BitStreamWriter::BitStreamWriter(std::string filePath) {
    fileWrite.open(filePath, ios::out | ios::binary | ios::in | ios::trunc);
    writeBuffer = nullptr;
    dataBeingProcessed = 0b00000000;
    bufferWriteBitPosition = 7;
}

BitStreamWriter::~BitStreamWriter(){
    paddAndCloseFile();
}

int BitStreamWriter::writeBit(int value) {
    if(value < 0 || value > 1) {
        paddAndCloseFile();
        return 0;
    }
    else{
        if(bufferWriteBitPosition < 0){
            bufferWriteBitPosition = 7;
            writeBuffer = &dataBeingProcessed;
            flushBufferContents();
            writeBit(value);    // we try once again
        }
        else {
            dataBeingProcessed |= value << bufferWriteBitPosition;
            bufferWriteBitPosition--;
            return 1;
        }
    }
    return 0;
}

void BitStreamWriter::flushBufferContents() {
    if(writeBuffer != nullptr){
        fileWrite.write(writeBuffer, 1);
        writeBuffer = nullptr;
        dataBeingProcessed = 0b00000000;
    }
}

void BitStreamWriter::paddAndCloseFile() {
    for(int i = bufferWriteBitPosition; i > 0; i--) {
        dataBeingProcessed |= 0 << i;
    }
    flushBufferContents();

    fileWrite.close();
}

int BitStreamWriter::writeNBits(int value) {
    /*
    int size = ceil(log2(value));
    int bitToWrite, result;
    for(int i = size; i >= 0; i--) {
        bitToWrite = (value & (1 << i)) >> i;
        result = writeBit(bitToWrite);
    }*/
    fileWrite.write(reinterpret_cast<const char *>(&value), 1);
    return 0;
}
