//
// Created by joelpinheiro on 17/11/15.
//

/**
 * videoCmp program, that compares two sequences in terms of
 * PSNR and root mean squared error:
 *
 * where A is the maximum value of the signal, e2 is the mean squared error
 * between the reconstructed frame, f, and the original frame, f, nd where
 * N and M denote, respectively, the number of rows and columns of the frames.
 */

#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iomanip>  // for controlling float print precision
#include <sstream>

using namespace std;
using namespace cv;

double getPSNR(const Mat& I1, const Mat& I2);

static void help()
{
    cout
    << "\n--------------------------------------------------------------------------" << endl
    << "Usage: referenceVideo compressedVIdeo wait_between_frames frame_size" << endl
    << "--------------------------------------------------------------------------"   << endl
    << endl;
}

int main(int argc, char** argv ){
    help();

    if(argc != 4){
        cout << "Not enough parameters" << endl;
        return -1;
    }

    stringstream conv;

    int delay;
    conv << argv[3] << endl;
    conv >> delay;

    int frame_size_Ref_V, frame_size_Ref_H, frame_size_Tst_V, frame_size_Tst_H;
    char c;
    int frameNum = -1;
    string tmp;

    ifstream captRefrnc(argv[1]);
    ifstream captUndTst(argv[2]);

    string refLine, tstLine;
    getline(captRefrnc, refLine);
    getline(captUndTst, tstLine);

    istringstream(refLine) >> frame_size_Ref_V;
    istringstream(refLine) >> frame_size_Ref_H;
    istringstream(tstLine) >> frame_size_Tst_V;
    istringstream(tstLine) >> frame_size_Tst_V;

    const char* WIN_UT = "Under Test";
    const char* WIN_RF = "Reference";

    // For them windows
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
    namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(WIN_RF, 400, 0);
    cvMoveWindow(WIN_UT, frame_size_Tst_V, 0);

    Mat frameReference(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC3);
    Mat frameUnderTest(Size(frame_size_Tst_V, frame_size_Tst_H), CV_8UC3);
    double psnrV;

    for(;;){    // keep showing them images

        if(!captRefrnc.read((char *) frameReference.ptr(),frame_size_Ref_V*frame_size_Ref_H*3) || !captUndTst.read((char *) frameUnderTest.ptr(),frame_size_Tst_V*frame_size_Tst_H*3)){
            cout << " < < < Game over! > > >";
            break;
        }

        ++frameNum;
        cout << "Frame: " << frameNum << "# ";

        // now for the PSNR
        cvtColor(frameReference, frameReference, CV_BGR2RGB);
        cvtColor(frameUnderTest, frameUnderTest, CV_BGR2RGB);

        psnrV = getPSNR(frameReference, frameUnderTest);
        cout << setiosflags(ios::fixed) << setprecision(3) << psnrV << " dB";

        cout << endl;

        // now we show both images for visual inspection
        imshow(WIN_RF, frameReference);
        imshow(WIN_UT, frameUnderTest);

        c = (char)cvWaitKey(delay);
        if(c == 27) break;
    }

    return 0;
}

double getPSNR(const Mat& I1, const Mat& I2){
    Mat s1;
    absdiff(I1, I2, s1);    //|I1 - I2|
    s1.convertTo(s1, CV_32F);   // make a square with 8 bits
    s1 = s1.mul(s1);    // |I1 - I2|²

    Scalar s = sum(s1); // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    double mse = sse / (double)(I1.channels() * I1.total());
    double psnr = 10.0 * log10((255 * 255) / mse);
    return psnr;

}