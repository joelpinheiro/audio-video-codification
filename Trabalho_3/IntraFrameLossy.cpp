//
// Created by root on 12/2/15.
//

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "RawIntraFrameCodecLossy.h"
#include <time.h>
#include <sys/time.h>

using namespace std;

static void help()
{
    cout
    << "\n------------------------------------------------------------------------------------------------" << endl
    << "Usage: referenceVideo compressedVIdeo decompressedVideo compressMode y_quantizer cr_quantizer cb_quantizer imageShow(0-f, 1-v)  M  " << endl
    << "--------------------------------------------------------------------------------------------------" << endl
    << endl;
}

int diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec))/1000;
}

int main(int argc, char** argv ) {
    help();

    if(argc != 10){
        cout << "Not enough parameters" << endl;
        return -1;
    }

    if(atoi(argv[4]) < 1 || atoi(argv[5]) < 1 || atoi(argv[6]) < 1){
        cout << "Wrong quantizer" << endl;
        return -1;
    }

    struct timeval startLossy, endLossy;

    gettimeofday (&startLossy, NULL);

    RawIntraFrameCodecLossy * encoder = new RawIntraFrameCodecLossy(argv[1], argv[2]);
    RawIntraFrameCodecLossy * decoder = new RawIntraFrameCodecLossy(argv[2], argv[3]);

    encoder->intraFrameEncode(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]), atoi(argv[9]));
    decoder->intraFrameDecode(atoi(argv[8]), atoi(argv[9]));

    gettimeofday (&endLossy, NULL);
    int diffEncode = diff_ms(endLossy, startLossy);
    printf ("\n\nTime (encoder+decoder)#: %d milisegundos.\n", diffEncode );

    printf("\n ------ \n");

    delete encoder;
    delete decoder;

    return 0;
}