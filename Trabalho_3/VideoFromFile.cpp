#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

static void help()
{
    cout
    << "\n--------------------------------------------------------------------------" << endl
    << "Usage: <File Path>"                                                                       << endl
    << "--------------------------------------------------------------------------"   << endl
    << endl;
}

int main(int argc, char* argv[])
{
    Mat dst;

    help();
    if (argc != 2)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    std::cout << argv[1] << std::endl;

    ifstream ifs(argv[1]);
    string line;
    getline(ifs, line);

    int frame_size_Ref_V, frame_size_Ref_H;


    istringstream iss(line);
    string s;

    getline( iss, s, ' ' );
    frame_size_Ref_V = atoi(s.c_str());

    getline( iss, s, ' ' );
    frame_size_Ref_H = atoi(s.c_str());


    // parse line
    Mat img(Size(frame_size_Ref_V,frame_size_Ref_H), CV_8UC3);

    while(ifs.read((char *) img.ptr(),frame_size_Ref_V*frame_size_Ref_H*3)) {
        cvtColor(img, dst, CV_BGR2RGB);
        imshow("frame",dst);
        waitKey(1);
    }

    return 0;
}
