//
// Created by root on 29-09-2015.
//

#include <iostream>
#include "../BitStream/BitStream.h"
#include "TextTranscoder.h"

using namespace std;

int main(){
    cout << "BitStream test, exercise 1_2" << endl;

    string filePath = "/root/ClionProjects/cav1516-65151-42967/BitStream_test/test_file_2";
    //string filePath = "/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/BitStream_test/test_file_2";

    string outputEncodeFilePath = "/root/ClionProjects/cav1516-65151-42967/BitStream_test/test_file_2_decoded";
    //string outputEncodeFilePath = "/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/BitStream_test/test_file_2_decoded";

    /*
     * We take a txt file, and decode it to a txt representstion of binary, and also straight to binary
     * */

    BitStream* fileHandleDecode = new BitStream(filePath, 0);     // we dont care about the buffer size, we let it choose one

    TextTranscoder decoder(fileHandleDecode, true);           // we want verbose mode enabled

    decoder.decodeFileBin(outputEncodeFilePath);

    delete fileHandleDecode;      // watch out for memory leaks


    /*
     * We take the binary file, and encode it back to txt format
     * */

    BitStream* fileHandleEncode = new BitStream(outputEncodeFilePath + ".binary", 0);     // we dont care about the buffer size, we let it choose one

    TextTranscoder encoder(fileHandleEncode, true);           // we want verbose mode enabled

    encoder.encodeFileBin(outputEncodeFilePath + ".encoded");

    delete fileHandleEncode;      // watch out for memory leaks



    /*
     * We take a txt file with 1s and 0s, and write the equivalent in binary
     * */

    BitStream* fileTextDecode = new BitStream(outputEncodeFilePath, 0);

    TextTranscoder decoderTextToBin(fileTextDecode, true);

    decoderTextToBin.decodeFileText(outputEncodeFilePath + ".textToBin");

    delete fileTextDecode;


    return 0;
}
