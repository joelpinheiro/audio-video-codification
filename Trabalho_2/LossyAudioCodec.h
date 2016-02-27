//
// Created by joelpinheiro on 27/10/15.
//

#ifndef TRABALHO2_LOSSYAUDIOCODEC_H
#define TRABALHO2_LOSSYAUDIOCODEC_H

#include "./Golomb/Golomb.h"

class LossyAudioCodec {
public:
    LossyAudioCodec(char* inputFile, char* outputFile, int m, int q, int predictor_level);
    ~LossyAudioCodec();
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
    int q;
    int predictor_level;
};


#endif //TRABALHO2_LOSSYAUDIOCODEC_H
