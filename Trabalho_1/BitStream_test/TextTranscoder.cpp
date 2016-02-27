//
// Created by root on 29-09-2015.
//

#include <iostream>
#include <fstream>
#include <bitset>
#include "TextTranscoder.h"

TextTranscoder::TextTranscoder(BitStream* fh, bool verbose){
    fileHandler = fh;
    isVerbose = verbose;
}

void TextTranscoder::decodeFileBin(std::string filePath) {
    if(isVerbose)
        std::cout << "decodeFileBin: Verbose output enabled for decoding in TextTranscoder" << std::endl;

    unsigned int fileSize = 0, counter = 0;

    std::string filePath_bin = filePath + ".binary";

    std::ofstream newFile_txt(filePath);

    BitStream decodedFile(filePath_bin, 0);
    decodedFile.openFileToWrite(CREATE_AND_REPLACE);

    fileHandler->openFileToRead();
    fileSize = fileHandler->getSize();
    if(isVerbose)
        std::cout << "decodeFileBin: Read file size is: " << fileSize << std::endl;

    unsigned int optimalBufferSize = fileHandler->getOptimalBufferSize();
    fileHandler->setBufferSize(optimalBufferSize);
    if(isVerbose)
        std::cout << "decodeFileBin: Optimal buffer size is: " << optimalBufferSize << std::endl;

    decodedFile.setBufferSize(optimalBufferSize);

    // we need a cycle to perform stream operations
    char* rBuff;

    while(counter < fileSize){
        fileHandler->readBit(counter);

        for(unsigned int i = 0; i < optimalBufferSize; i++){

            for(unsigned int j = 0; j < 8; j++){
                newFile_txt << fileHandler->getBitToText(i, j);

            }
            rBuff = fileHandler->getBufferContents();
            decodedFile.setBufferContents(rBuff);
            decodedFile.writeBit(counter, optimalBufferSize);
        }

        counter += optimalBufferSize;

        if(isVerbose)
            std::cout << "decodeFileBin: Processed " << counter << " bytes." << std::endl;
    }

    newFile_txt.close();
    decodedFile.closeFileWrite();
    fileHandler->closeFileRead();
}

void TextTranscoder::encodeFileBin(std::string outputEncodeFilePath) {

    if(isVerbose)
        std::cout << "encodeFileBin: Verbose output enabled for encoding in TextTranscoder" << std::endl;

    fileHandler->openFileToRead();

    BitStream encodedFile(outputEncodeFilePath, 0);
    encodedFile.openFileToWrite(CREATE_AND_REPLACE);

    unsigned int bufferSize = fileHandler->getOptimalBufferSize();
    fileHandler->setBufferSize(bufferSize);
    encodedFile.setBufferSize(bufferSize);

    unsigned int counter = 0;

    if(isVerbose)
        std::cout << "encodeFileBin: Buffer read file size is: " << fileHandler->getBufferSize() << std::endl;
    if(isVerbose)
        std::cout << "encodeFileBin: Buffer write file size is: " << encodedFile.getBufferSize() << std::endl;


    unsigned int fileSize = fileHandler->getSize();
    if(isVerbose)
        std::cout << "encodeFileBin: Read file size is: " << fileSize << std::endl;

    char* rBuff;

    while(counter < fileSize){
        fileHandler->readBit(counter);
        rBuff = fileHandler->getBufferContents();
        encodedFile.setBufferContents(rBuff);
        encodedFile.writeBit(counter, bufferSize);

        counter += bufferSize;

        if(isVerbose)
            std::cout << "encodeFileBin: Processed " << counter << " bytes." << std::endl;
    }

    encodedFile.closeFileWrite();
    fileHandler->closeFileRead();
}

void TextTranscoder::decodeFileText(std::string filePath) {
    if(isVerbose)
        std::cout << "decodeFileText: Verbose output enabled for encoding in TextTranscoder" << std::endl;

    std::string filePath_binFromText = filePath;

    fileHandler->openFileToRead();
    unsigned int buffSize = 8;      // we read only one byte at a time for now
    unsigned int fileSize = fileHandler->getSize();
    fileHandler->setBufferSize(buffSize);
    if(isVerbose)
        std::cout << "decodeFileText: Read file size is: " << fileSize << std::endl;

    BitStream decodedFile(filePath_binFromText, 0);
    decodedFile.openFileToWrite(CREATE_AND_REPLACE);
    decodedFile.setBufferSize(1);

    if(isVerbose)
        std::cout << "decodeFileText: Buffer read file size is: " << fileHandler->getBufferSize() << std::endl;
    if(isVerbose)
        std::cout << "decodeFileText: Buffer write file size is: " << decodedFile.getBufferSize() << std::endl;

    char* rBuff;
    char value;
    unsigned int counter = 0;
    unsigned int byteCounter = 0;

    while(counter < fileSize){

        fileHandler->readBit(counter);
        rBuff = fileHandler->getBufferContents();
        value = 0b00000000;
        for(unsigned int i = 0; i < buffSize; i++){
            if(rBuff[i] == '1'){
                if(i == 0)
                    value |= 0b00000001;
                else if(i == 1)
                    value |= 0b00000010;
                else if(i == 2)
                    value |= 0b00000100;
                else if(i == 3)
                    value |= 0b00001000;
                else if(i == 4)
                    value |= 0b00010000;
                else if(i == 5)
                    value |= 0b00100000;
                else if(i == 6)
                    value |= 0b01000000;
                else if(i == 7)
                    value |= 0b10000000;
            }
        }

        decodedFile.setBufferContents(&value);
        decodedFile.writeBit(byteCounter, 1);

        byteCounter++;
        counter += buffSize;
        if(isVerbose)
            std::cout << "decodeFileText: Processed " << counter << " bytes." << std::endl;
    }

    fileHandler->closeFileRead();
    decodedFile.closeFileWrite();
}
