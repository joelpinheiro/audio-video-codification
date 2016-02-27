//
// Created by joelpinheiro on 15/10/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "LosslessAudioCodec.h"
#include "LossyAudioCodec.h"
#include <math.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

int diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec))/1000;
}

static void DisplayProperUsage(FILE * pFile)
{
    fprintf(pFile, "Usage: [wav original] [wav lossless]  [wav lossy]  [M] [Q] [PREDICTION_LEVEL]\n\n");

    fprintf(pFile, "Examples:\n");
    fprintf(pFile, "    ./CMakeLists.txt ./Trabalho_2/Samples/sample07.wav ./Trabalho_2/Samples/Lossless.wav ./Trabalho_2/Samples/Lossy.wav 480 50 1\n");
}

int main(int argc, char **argv)
{


    SNDFILE *soundFileIn;   // Pointer for input sound file
    SNDFILE *soundFileOut;  // Pointer for output sound file
    SF_INFO soundInfoIn;    // Input sound file Info
    SF_INFO soundInfoOut;   // Output sound file Info

    int i;
    short sample[2];
    sf_count_t nSamples = 1;

    if (argc != 7){
        DisplayProperUsage(stderr);
        return -1;
    }

    /* When opening a file for read, the format field should be set to zero
     * before calling sf_open(). All other fields of the structure are filled
     * in by the library
     */
    soundInfoIn.format = 0;
    soundFileIn = sf_open (argv[1], SFM_READ, &soundInfoIn);

    if (soundFileIn == NULL){
        fprintf(stderr, "Could not open file for reading: \"%s\"\n",
                argv[1]);
        sf_close(soundFileIn);
        return -1;
    }


    /* When opening a file for write, it is necessary to fill in the
     * structure members samplerate, channels and format.
     */
    soundInfoOut.samplerate = soundInfoIn.samplerate;
    soundInfoOut.channels = soundInfoIn.channels;
    soundInfoOut.format = soundInfoIn.format;

    soundFileOut = sf_open (argv[2], SFM_WRITE, &soundInfoOut);

    if (soundFileOut == NULL){
        fprintf(stderr, "Could not open file for writing: \"%s\"\n", argv[2]);
        sf_close(soundFileIn);
        return -1;
    }


    // Structure to save Samples and couting of each sample
    map<int, int> m = {};

    for (i = 0; i < soundInfoIn.frames ; i++)
    {
        if (sf_readf_short(soundFileIn, sample, nSamples) == 0){
            fprintf(stderr, "Error: Reached end of file\n");
            sf_close(soundFileIn);
            sf_close(soundFileOut);
            break;
        }

        int sample_channel_1 = sample[0];
        int sample_channel_2 = sample[1];

        m[sample_channel_1] += 1;
        m[sample_channel_2] += 1;

    }

    sf_close(soundFileIn);
    sf_close(soundFileOut);

    std::ofstream myfile;
    myfile.open ("./plot_data.dat");

    myfile << "# This file is called   plot_data.dat" << std::endl;
    myfile << "# Histogram: ocurrencies of n sample" << std::endl;
    myfile << "# Sample Number" << std::endl;

    for(auto elm : m)
        myfile << elm.first << "\t" << elm.second <<"\t" << elm.first << std::endl;
    myfile.close();

    FILE *file;

    file = popen("gnuplot -persist ", "w");
    fprintf(file, "set grid\n"
            "set boxwidth 0.95 relative\n"
            "set style data histograms\n"
            "set style fill transparent solid 0.5 noborder\n"
            "plot \"./plot_data.dat\" u 1:2 w boxes lc rgb\"green\" notitle\n");


    std::cout << "\nLosslessAudioCodec Begin:" << std::endl;


    /**
     * Melhor M e' 480
     */
    int M = atoi(argv[4]);
    int Q = atoi(argv[5]);
    int predictor_level = atoi(argv[6]);

    struct timeval startLossless, endLossless, startLossy, endLossy;
    gettimeofday (&startLossless, NULL);

    LosslessAudioCodec lossless(argv[1],argv[2],M,predictor_level);
    lossless.Encode();
    lossless.Decode();

    int *entangledArray = lossless.entangledArray;
    int *entangledDecodedArray = lossless.entangledDecodedArray;
    int size = lossless.entangledArraysSize;

    double sumXi = 0;
    double sumXiXri = 0;
    for(i = 0 ; i < size ; i++)
    {
        sumXi = sumXi + pow(entangledArray[i], 2);
        sumXiXri = sumXiXri + ( pow((entangledArray[i] - entangledDecodedArray[i]), 2) ) ;
    }

    double PSNR = 10 * log10(sumXi / sumXiXri);

    gettimeofday (&endLossless, NULL);

    int diffEncode = diff_ms(endLossless, startLossless);
    printf ("Tempo de codificação e descodificação do Codec Lossless: %d milisegundos.\n", diffEncode );

    std::ifstream file_gulomb("./tmpGulombLossless", std::ifstream::ate | std::ifstream::binary);
    int fileSizeEncodedGulomb = (int)file_gulomb.tellg();

    printf("Tamanho do ficheiro Gulomb Lossless codificado e: %d\n", fileSizeEncodedGulomb);

    std::cout << "Lossless PSNR Lossless: " << PSNR << std::endl;

    std::cout << "\nLossyAudioCodec Begin:" << std::endl;

    gettimeofday (&startLossy, NULL);

    LossyAudioCodec lossy(argv[1],argv[3], M, Q, predictor_level);
    lossy.Encode();
    lossy.Decode();

    sumXi = 0;
    sumXiXri = 0;
    entangledArray = lossy.entangledArray;
    entangledDecodedArray = lossy.entangledDecodedArray;
    // 10 * log10(somatorio[sampleOriginal^2] / somatorio[(sampleOriginal - sampleDecoded)^2])
    for(i = 0 ; i < size ; i++)
    {
        sumXi = sumXi + pow(entangledArray[i], 2);
        sumXiXri = sumXiXri + ( pow((entangledArray[i] - entangledDecodedArray[i]), 2) ) ;
    }
    PSNR = 10 * log10(sumXi / sumXiXri) * -1;

    gettimeofday (&endLossy, NULL);

    diffEncode = diff_ms(endLossy, startLossy);
    printf ("Tempo de codificação e descodificação do Codec Lossy: %d milisegundos.\n", diffEncode );

    std::ifstream file_gulomb2("./tmpGulombLossy", std::ifstream::ate | std::ifstream::binary);
    fileSizeEncodedGulomb = (int)file_gulomb2.tellg();

    printf("Tamanho do ficheiro Gulomb Lossy codificado e: %d\n", fileSizeEncodedGulomb);

    std::cout << "Lossy PSNR Lossy: " << PSNR << std::endl;

    return 0;
}
