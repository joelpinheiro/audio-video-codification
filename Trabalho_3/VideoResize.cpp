//
// Created by joelpinheiro on 17/11/15.
//

/**
 * Write a program, named videoResize, that can perform spatial resolution reduction,
 * as well as spatial resolution expansion. For example, a reduction of 1:2 transforms
 * a video with frames of N × M pixels into another with N/2 × M/2) and an expansion of
 * 1:2 transforms a N × M video into one of resolution 2N × 2M pixels).
 */

#include "opencv2/opencv.hpp"
#include <fstream>



using namespace cv;
using namespace std;

static void help()
{
    cout
    << "\n--------------------------------------------------------------------------" << endl
    << "Usage: referenceVideo resizedVIdeo verticalRes horizontalRes" << endl
    << "--------------------------------------------------------------------------"   << endl
    << endl;
}

int main(int argc, char** argv ){
    help();

    if(argc != 5){
        cout << "Not enough parameters" << endl;
        return -1;
    }

    ifstream ifs(argv[1]);
    string line;
    getline(ifs, line);
    // parse line

    int frame_size_Ref_V, frame_size_Ref_H, frameRate;
    istringstream(line) >> frame_size_Ref_V;
    istringstream(line) >> frame_size_Ref_H;
    istringstream(line) >> frameRate;

    Mat img(Size(frame_size_Ref_V,frame_size_Ref_H), CV_8UC3);
    Mat res(Size(atoi(argv[3]), atoi(argv[4])), CV_8UC3);

    ofstream output(argv[2], ios::trunc | ios::binary);

    output << line << std::endl;

    Vec3b pInit;

    //res.zeros(res.size(), CV_8UC3);
    res = Scalar(0);

    int resizeRatio_V = (frame_size_Ref_V) / (atoi(argv[3]));
    int resizeRatio_H = (frame_size_Ref_H) / (atoi(argv[4]));
    int x=0, y=0;

    int frameCount = -1;

    // for the visuaisation windows
    const char* WIN_UT = "Compressed";
    const char* WIN_RF = "Original";
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
    namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(WIN_RF, frame_size_Ref_V, 0);
    cvMoveWindow(WIN_UT, atoi(argv[3]), 0);

    while(ifs.read((char *) img.ptr(), frame_size_Ref_V*frame_size_Ref_H*3)) {
        cvtColor(img, img, CV_BGR2RGB);
        frameCount++;

        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                pInit = img.at<Vec3b>(i, j);
                x = i / resizeRatio_H;
                y = j / resizeRatio_V;
                res.at<Vec3b>(x, y) = pInit;
            }

        }
        if(output.is_open()){
            output.write((char*) res.ptr(), atoi(argv[3]) * atoi(argv[4]) * 3);
        }
            //output << res.data;
        else{
            cout << "Error opening file" << endl;
            return -1;
        }

        imshow(WIN_UT, res);
        imshow(WIN_RF, img);
        waitKey(100);
        cout << "Frame#: " << frameCount << endl;
    }

    return 0;
}