//
// Created by root on 05-10-2015.
//

#ifndef GOLOMB_BITSTREAMWRITER_H
#define GOLOMB_BITSTREAMWRITER_H

#include <iostream>
#include <fstream>

class BitStreamWriter {
public:
    BitStreamWriter(std::string filePath);
    int writeBit(int value);
    int writeNBits(int value);
    ~BitStreamWriter();
    void paddAndCloseFile();
protected:
private:
    void flushBufferContents();
    std::ofstream fileWrite;
    char* writeBuffer;
    char dataBeingProcessed;
    int bufferWriteBitPosition;
};


#endif //GOLOMB_BITSTREAMWRITER_H
