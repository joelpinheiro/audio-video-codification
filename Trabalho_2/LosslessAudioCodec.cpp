//
// Created by root on 20-10-2015.
//

#include "LosslessAudioCodec.h"
#include <iostream>
#include <sndfile.h>
#include <map>
#include <vector>


LosslessAudioCodec::LosslessAudioCodec(char* inputFile, char* outputFile, int m, int predictor_level) {
    gEncoder = new GolombEncoder("./tmpGulombLossless", m);
    gDecoder = new GolombDecoder("./tmpGulombLossless", m);
    this->inputFile = inputFile;
    this->outputFile = outputFile;
    this->m = m;
    this->predictor_level = predictor_level;
}

LosslessAudioCodec::~LosslessAudioCodec() {
    delete(gDecoder);
    delete(gEncoder);
}

void LosslessAudioCodec::Encode() {

    SNDFILE *soundFileIn;   // Pointer for input sound file
    SF_INFO soundInfoIn;    // Input sound file Info

    int i;

    short sample[2];
    sf_count_t nSamples = 1;

    /* When opening a file for read, the format field should be set to zero
     * before calling sf_open(). All other fields of the structure are filled
     * in by the library
     */
    soundInfoIn.format = 0;
    soundFileIn = sf_open (inputFile, SFM_READ, &soundInfoIn);

    if (soundFileIn == NULL){
        fprintf(stderr, "Could not open file for reading: \"%s\"\n",
                inputFile);
        sf_close(soundFileIn);
    }

    int sample_rate = soundInfoIn.samplerate;
    int channels = soundInfoIn.channels;
    int format = soundInfoIn.format;

//    fprintf(stderr, "Frames (Samples): %d\n", (int) soundInfoIn.frames);
//    fprintf(stderr, "Samplerate: %d\n", soundInfoIn.samplerate);
//    fprintf(stderr, "Channels: %d\n", soundInfoIn.channels);
//    fprintf(stderr, "Format: %d\n", soundInfoIn.format);
//    fprintf(stderr, "Sections: %d\n", soundInfoIn.sections);


    int number_of_frames =  (int) soundInfoIn.frames;

    int **diffArray;
    diffArray = new int*[number_of_frames];
    for(int i = 0; i < number_of_frames; ++i){
        diffArray[i] = new int[2];
    }

    int previousSample = 0;


    int *entangledSamples = new int[number_of_frames*2];
    int *entangledDiffArray = new int[number_of_frames*2];


    /******************************************************************************************************************/
    int j = 0;
    for(i = 0; i < soundInfoIn.frames; i++)
    {
        if (sf_readf_short(soundFileIn, sample, nSamples) == 0){
            fprintf(stderr, "Error: Reached end of file\n");
            sf_close(soundFileIn);
            break;
        }

        entangledSamples[j] = sample[0];
        entangledSamples[j+1] = sample[1];
        j += 2;
    }

    if (this->predictor_level == 1) {
        /*
         * Modelo utilizando apenas redundancia temporal, onde:
         * r(0) = f(0)              , n = 0
         * r(1) = f(1)              , n = 1
         * r(n) = f(n) - f(n-2)     , n >= 2
         */
        entangledDiffArray[0] = entangledSamples[0] - 0;
        entangledDiffArray[1] = entangledSamples[1] - 0;
        for (int i = 2; i < number_of_frames * 2; i++) {
            entangledDiffArray[i] = entangledSamples[i] - entangledSamples[i - 2];
        }
    }
    else {

        entangledDiffArray[0] = entangledSamples[0];
        entangledDiffArray[1] = entangledSamples[1] - entangledSamples[0];
        for (int i = 2; i < number_of_frames * 2; i++) {
            entangledDiffArray[i] = entangledSamples[i] - ((entangledSamples[i - 1] + entangledSamples[i - 2]) / 2);
        }
    }

    /******************************************************************************************************************/



//    std::ofstream myfile;
//    myfile.open ("./plot_data.dat");
//
//    myfile << "# This file is called   plot_data.dat" << std::endl;
//    myfile << "# Histogram: ocurrencies of n sample" << std::endl;
//    myfile << "# Sample Number" << std::endl;

    std::ofstream tmpLossless;
    tmpLossless.open ("./tmpLossless");

    map<int, int> map_counter = {};

    for(i = 0; i < number_of_frames * 2 ; i++){

        map_counter[entangledDiffArray[i]] += 1;

        tmpLossless << entangledDiffArray[i];

        if(i < ((number_of_frames * 2) - 1))
            tmpLossless << ",";
    }

    sf_close(soundFileIn);
//    myfile.close();
    tmpLossless.close();

    std::ofstream myfile;
    myfile.open ("./plot_data_diff.dat");

    myfile << "# This file is called   plot_data.dat" << std::endl;
    myfile << "# Histogram: ocurrencies of n sample" << std::endl;
    myfile << "# Sample Number" << std::endl;

    for(auto elm : map_counter)
        myfile << elm.first << "\t" << elm.second <<"\t" << elm.first << std::endl;
    myfile.close();

    FILE *file;

    file = popen("gnuplot -persist ", "w");
    fprintf(file, "set grid\n"
            "set boxwidth 0.95 relative\n"
            "set style data histograms\n"
            "set style fill transparent solid 0.5 noborder\n"
            "plot \"./plot_data_diff.dat\" u 1:2 w boxes lc rgb\"pink\" notitle\n");



    /*
     * Ler Wav
     * Fazer operacoes com os valores
     * Escrever os resultados em bistream num ficheiro -> lEncodeInputFile
     * Depois de concluido fechar o ficheiro -> lEncodeInputFile
     * golomb abre -> lEncodeInputFile
     * golomb codifica -> gEncodeInputFile
     * golomb fecha o ficheiro e escreve em -> gEncodeOutputFile
     */

    /*
     * Lossless encoding ja esta feita, agora passamos ao golomb
     * */

    int m, x;

    m = this->m;
    ifstream inpFile("./tmpLossless");

    if(!inpFile.is_open()) {
        std::cout << "Error: could not open file" << endl;
    }

    // Count symbols to encode
    int n = 0;
    int c = ',';
    while(c == ',') {
        inpFile >> x;
        c = inpFile.get();
        n++;
    }

    inpFile.clear();
    inpFile.seekg(0, ios::beg);

    gEncoder->WriteString(to_string(m));
    gEncoder->WriteString(to_string(n));
    gEncoder->WriteString(to_string(sample_rate));
    gEncoder->WriteString(to_string(channels));
    gEncoder->WriteString(to_string(format));

    c = ',';
    while(c == ',') {
        inpFile >> x;
        c = inpFile.get();
        gEncoder->WriteInt(abs(x));
        if(x != 0) {
            if(x < 0)
                gEncoder->WriteBit(1);

            else
                gEncoder->WriteBit(0);

        }

    }

    gEncoder->Close();

    this->entangledArray = entangledSamples;
    this->entangledArraysSize = number_of_frames * 2;

}

void LosslessAudioCodec::Decode() {

    /**
     * Ler ficheiro codificado por Golomb -> tmpGolomb
     * Descodificar ficheiro Golomb para ter codificado lossless -> tmpLossless
     * Ler codificado lossless -> tmpLossless
     * Descofificar ficheiro lossless -> tmpLossless
     * Escrever cópia do ficheiro original
     */

    int x, n;

    ofstream losslessEncodedFile("./tmpLosslessDecoded");

    int k = atoi(gDecoder->ReadString().c_str());

    gDecoder->SetParameter(k); // reads m from the file
    n = atoi(gDecoder->ReadString().c_str()); // reads the number of integers to decode

    int sample_rate = atoi(gDecoder->ReadString().c_str()); // reads the sample_rate;
    int channels = atoi(gDecoder->ReadString().c_str()); // reads the channels;
    int format = atoi(gDecoder->ReadString().c_str()); // reads the format;

    /**/
    int *entangledDiffArray = new int[n];
    /**/


    for(int i = 0 ; i < n ; i++) {
        x = gDecoder->ReadInt();
        if(x != 0 && gDecoder->ReadBit())
            x *= -1;

        losslessEncodedFile << x;

        /**/
        entangledDiffArray[i] = x;
        /**/

        if(i != n - 1) {
            losslessEncodedFile << ",";
        }
    }

    losslessEncodedFile.close();
    gDecoder->Close();

    int *entangledSamples = new int[n];

    if (this->predictor_level == 1) {
        /*
         * Modelo utilizando apenas redundancia temporal, onde:
         * f(0) = r(0)          , n = 0
         * f(n) = r(n) + r(n-1) , n > 0
         */
        entangledSamples[0] = entangledDiffArray[0] + 0;
        entangledSamples[1] = entangledDiffArray[1] + 0;
        for (int i = 2; i < n; i++) {
            entangledSamples[i] = entangledDiffArray[i] + entangledSamples[i - 2];
        }
    }
    else {

        entangledSamples[0] = entangledDiffArray[0];
        entangledSamples[1] = entangledDiffArray[1] + entangledDiffArray[0];
        for (int i = 2; i < n; i++) {
            entangledSamples[i] = entangledDiffArray[i] + ((entangledSamples[i - 1] + entangledSamples[i - 2]) / 2);
        }
    }


    SNDFILE *soundFileOut;  // Pointer for output sound file
    SF_INFO soundInfoOut;   // Output sound file Info

    int i;
    short sample[2];
    sf_count_t nSamples = 1;

    /* When opening a file for write, it is necessary to fill in the
     * structure members samplerate, channels and format.
     */
    soundInfoOut.samplerate = sample_rate;
    soundInfoOut.channels = channels;
    soundInfoOut.format = format;

    soundFileOut = sf_open (outputFile, SFM_WRITE, &soundInfoOut);

    if (soundFileOut == NULL){
        fprintf(stderr, "Could not open file for writing: \"%s\"\n", outputFile);
        sf_close(soundFileOut);
    }

    int j = 0;
    for (i = 0; i < n / 2 ; i++)
    {
        sample[0] = entangledSamples[j];
        sample[1] = entangledSamples[j+1];

        if (sf_writef_short(soundFileOut, sample, nSamples) != 1){
            fprintf(stderr, "Error writing frames to the output:\n");
            sf_close(soundFileOut);
        }

        j += 2;
    }

    sf_close(soundFileOut);

    this->entangledDecodedArray = entangledSamples;
}
