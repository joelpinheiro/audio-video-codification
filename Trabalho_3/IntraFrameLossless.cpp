//
// Created by root on 12/2/15.
//

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "RawIntraFrameCodecLossless.h"
#include <time.h>
#include <sys/time.h>

using namespace std;

static void help()
{
    cout
    << "\n------------------------------------------------------------------------------------------------" << endl
    << "Usage: referenceVideo compressedVIdeo decompressedVideo compressMode imageShow(0-f, 1-v) m " << endl
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

    if(argc != 7){
        cout << "Not enough parameters" << endl;
        return -1;
    }

    if(atoi(argv[6]) < 1){
        cout << "Wrong M" << endl;
        return -1;
    }

    printf("Reference Video", argv[1]);

    RawIntraFrameCodecLossless * encoder = new RawIntraFrameCodecLossless(argv[1], argv[2]);
    RawIntraFrameCodecLossless * decoder = new RawIntraFrameCodecLossless(argv[2], argv[3]);

    struct timeval startLossless, endLossless;

    gettimeofday (&startLossless, NULL);

    encoder->intraFrameEncode(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    decoder->intraFrameDecode(atoi(argv[4]), atoi(argv[5]));

    gettimeofday (&endLossless, NULL);
    int diffEncode = diff_ms(endLossless, startLossless);
    printf ("\n\nTime (encoder+decoder)#: %d milisegundos.\n", diffEncode );

    printf("\n ------ \n");

    delete encoder;
    delete decoder;

    return 0;
}