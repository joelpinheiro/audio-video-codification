//
// Created by root on 12/2/15.
//

#ifndef CAV1516_RAWINTRAFRAMECODEC_H
#define CAV1516_RAWINTRAFRAMECODEC_H
#include "opencv2/opencv.hpp"

using namespace cv;

class RawIntraFrameCodecLossy {
public:
    RawIntraFrameCodecLossy(char* inputFilePath, char* outputFilePath);
    void intraFrameEncode(int predictorMode, int q_y, int q_cr, int q_cb, int showVideo, int m);
    void intraFrameDecode(int predictorMode, int showVideo);

protected:
private:
    Mat* img;
    Mat* intermediate;

    Mat* resize;

    Mat* predY;
    Mat* predU;
    Mat* predV;

    Mat* yyMat;

    Mat* uuMat;
    Mat* vvMat;

    char* inputFilePath;
    char* outputFilepath;
    Mat* resizeFrame(Mat* matToResize);
    void predictMat(int predictorMode);
    short predictValue(int predictorMode, Mat *xxMat, int i, int j);
};

#endif //CAV1516_RAWINTRAFRAMEENCODER_H
