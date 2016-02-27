//
// Created by root on 30-09-2015.
//
#include <iostream>
#include <sstream>
#include <bitset>
#include <time.h>
#include <fstream>
#include "Golomb.h"
#include <time.h>
#include <sys/time.h>

int diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec))/1000;
}

int main(){


    std::cout << "Golomb Test File" << std::endl;

    BitStreamReader *bitStreamReader;
    BitStreamWriter *bitStreamWriter;

    const int M = 523;

    std::string testFilePath = "/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/Golomb/ecgDIFF.csv";
    std::string encodedFilePath = "/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/Golomb/output_file";
    std::string decodedFilePath = "/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/Golomb/ecgDIFF_decoded.csv";

    bitStreamReader = new BitStreamReader(testFilePath);
    bitStreamWriter = new BitStreamWriter(encodedFilePath);

    int readBit = 0;
    int n_size_bytes = 1;
    int n_size = n_size_bytes * 8;
    std::string n_string = "";
    Golomb *golomb = new Golomb();

    struct timeval startEncode, endEncode, startDecode, endDecode;
    gettimeofday (&startEncode, NULL);

    while(readBit != 37)
    {
        for(int i = 0; i < n_size ; i++)        // Call golombEncode with a byte (8 bits)
        {
            readBit = bitStreamReader->readBit();
            n_string = n_string + std::to_string(readBit);
        }
        if(n_string.compare("3737373737373737") == 0) {
            n_string = "00000000";
        }
        //std::cout << "Sending bits to encode: " << n_string << std::endl;
        golomb->golombEncode(M, n_string, bitStreamWriter);

        n_string = "";
    }

    gettimeofday (&endEncode, NULL);

    int diffEncode = diff_ms(endEncode, startEncode);
    printf ("Tempo de codificacao: %d milisegundos.\n", diffEncode );

    std::ifstream inEncoded(encodedFilePath, std::ifstream::ate | std::ifstream::binary);
    std::ifstream original(testFilePath, std::ifstream::ate | std::ifstream::binary);

    int fileSizeEncoded = (int)inEncoded.tellg();
    int fileSizeOriginal = (int)original.tellg();

    printf("Tamanho do ficheiro codificado e: %d\n", fileSizeEncoded);
    printf("Tamanho do ficheiro original e: %d\n", fileSizeOriginal);

    inEncoded.close();
    original.close();

    double taxa_compressao = 100 * fileSizeEncoded / fileSizeOriginal;
    std::cout << "Taxa de compressão (%): " << taxa_compressao << std::endl;

    delete bitStreamWriter;

    /**
     * Decode Golomb
     */
    bitStreamReader = new BitStreamReader(encodedFilePath);
    bitStreamWriter = new BitStreamWriter(decodedFilePath);

    gettimeofday (&startDecode, NULL);

    int writeBitDecoded = 0;
    int error = 0;


    while(error != -1){
        writeBitDecoded = golomb->golombDecode(M, bitStreamReader, error);

        /*
        if(error != -1)
            break;
        */

        bitStreamWriter->writeNBits(writeBitDecoded);
    }
    bitStreamWriter->paddAndCloseFile();

    gettimeofday (&endDecode, NULL);
    int diffDecode = diff_ms(endDecode, startDecode);

    printf ("Tempo de descodificacao: %d milisegundos.\n", diffDecode );

    std::ifstream inDecoded(decodedFilePath, std::ifstream::binary | std::ifstream::ate);
    printf("Tamanho do ficheiro Descodificado e: %d", (int)inDecoded.tellg());
    inDecoded.close();

    delete bitStreamReader;
    delete bitStreamWriter;
    delete golomb;
    return 0;
}

