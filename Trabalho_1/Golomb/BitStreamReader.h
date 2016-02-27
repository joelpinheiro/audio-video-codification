//
// Created by root on 05-10-2015.
//

#ifndef GOLOMB_BITSTREAMREADER_H
#define GOLOMB_BITSTREAMREADER_H

#include <iostream>
#include <fstream>


class BitStreamReader {
public:
    BitStreamReader(std::string filePath);
    int readBit();
protected:
private:
    unsigned int getBufferSize();
    void refreshBufferContents();
    std::ifstream fileRead;
    char* readBuffer;
    unsigned int bufferSize;
    unsigned int fileSize;
    unsigned int fileReadPosition;
    int bufferReadBitPosition;
    unsigned int bufferReadBytePosition;
};


#endif //GOLOMB_BITSTREAMREADER_H
