//
// Created by root on 30-09-2015.
//

#include "Golomb.h"
#include <bitset>
#include <cmath>
#include <sstream>

Golomb::Golomb() {

}

void Golomb::golombEncode(unsigned int m, std::string n_string, BitStreamWriter *bitStreamWriter) {
    int q, r;
    int b = ceil(log2(m));
    int l = pow(2,b) - m;
    int n = 0;

    int signal;

    // std::cout << "Inputed integer in binary: " <<n_string << std::endl;

    char *ptr;
    int value = std::strtol(n_string.c_str(), &ptr, 2);

    // std::cout << "Inputed integer in decimal: "  << value << std::endl;

    if(value >= 0)
        signal = 0;
    else
        signal = 1;
    value = abs(value);

    n = value;

    // Print ASCII
    std::stringstream sstream(n_string);
    std::string output;
    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }

    // std::cout << "Inputed integer in ascii: " << output << std::endl;


    // std::cout << "Gulomb Code: ";

    q = n / m;
    r = n % m;

    for (int i = 0 ; i < q ; i++)
    {
        bitStreamWriter->writeBit(0);
        // std::cout << 0;
    }

    if(n_string.compare("00000000") != 0)
        bitStreamWriter->writeBit(1);

    // std::cout << 1;


    int mask = 1;
    if (r < l)
    {
        for (int i = b - 2; i >= 0; i--)
        {
            bitStreamWriter->writeBit((r >> i) & mask);
            // std::cout << ((r >> i) & mask);
        }
    }
    else
    {
        int aux = r + l;
        for (int i = b - 1; i >= 0; i--)
        {
            bitStreamWriter->writeBit((aux >> i) & mask);
            // std::cout << ((aux >> i) & mask);
        }
    }

    if(n != 0)
        bitStreamWriter->writeBit(signal);




    if(n_string.compare("00000000") == 0)
        bitStreamWriter->paddAndCloseFile();

    // std::cout << std::endl << std::endl;
}


int Golomb::golombDecode(const int M, BitStreamReader *bitStreamReader, int &error) {
    int b = ceil(log2(M));
    int l = pow(2, b) - M;
    //int h = M - l;
    int q = 0, r = 0;
    int result;
    int bitToRead = bitStreamReader->readBit();// = bitStreamReader->readBit();


    while (bitToRead != 1) {
        q++;
        bitToRead = bitStreamReader->readBit();

        if (bitToRead == 37) {
            error = -1;  // EOF
            q = 0;
            b = 0;
            l = 0;
            break;
        }

    }


    /*
    for (int i = b; i > 0; i--) {
        bitToRead = bitStreamReader->readBit();
        r |=  bitToRead << i-1;
    }
     */

    for (int i = 0; i < b - 1; i++) {

        bitToRead = bitStreamReader->readBit();

        r = (r << 1) | bitToRead;

    }

    if (r >= l) {
        bitToRead = bitStreamReader->readBit();
        r = ((r << 1) | bitToRead) - l;
    }


    result = q * M + r;

    if (result != 0) {
        bitToRead = bitStreamReader->readBit();
        if (bitToRead == 1)
            result = -result;
    }

    char string = static_cast<char>(result);

     //std::cout << "Decoded set: " << string << " In integer: " << result << std::endl;


    return result;
}
