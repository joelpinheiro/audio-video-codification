//
// Created by root on 29-09-2015.
//

#ifndef BITSTREAM_TEST_TEXTTRANSCODER_H
#define BITSTREAM_TEST_TEXTTRANSCODER_H

#include "../BitStream/BitStream.h"

class TextTranscoder {
public:
    TextTranscoder(BitStream* fh, bool verbose);
    void decodeFileBin(std::string filePath);
    void encodeFileBin(std::string outputEncodeFilePath);
    void decodeFileText(std::string filePath);
protected:
private:
    BitStream *fileHandler;
    bool isVerbose;
};


#endif //BITSTREAM_TEST_TEXTTRANSCODER_H
