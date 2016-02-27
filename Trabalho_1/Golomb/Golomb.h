//
// Created by root on 30-09-2015.
//

#ifndef GOULOMB_GOULOMB_H
#define GOULOMB_GOULOMB_H

#include "BitStreamReader.h"
#include "BitStreamWriter.h"


class Golomb {
public:
    Golomb();
    void golombEncode(unsigned int m, std::string n_string, BitStreamWriter *bitStreamWriter);
    int golombDecode(const int m, BitStreamReader *bitStreamReader, int &error);
protected:
private:
    BitStreamReader *bitStreamReader;
    BitStreamWriter *bitStreamWriter;
};


#endif //GOULOMB_GOULOMB_H
