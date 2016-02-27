//
// Created by root on 20-10-2015.
//

#ifndef TRABALHO2_LOSSLESSAUDIOCODEC_H
#define TRABALHO2_LOSSLESSAUDIOCODEC_H

#include "./Golomb/Golomb.h"


class LosslessAudioCodec {
public:
    LosslessAudioCodec(char* inputFile, char* outputFile, int m, int predictor_level);
    ~LosslessAudioCodec();
    void Encode();
    void Decode();
    int *entangledArray;
    int *entangledDecodedArray;
    int entangledArraysSize;
protected:
private:
    GolombEncoder* gEncoder;
    GolombDecoder* gDecoder;
    char* inputFile;
    char* outputFile;
    int m;
    int predictor_level;
};


#endif //TRABALHO2_LOSSLESSAUDIOCODEC_H
