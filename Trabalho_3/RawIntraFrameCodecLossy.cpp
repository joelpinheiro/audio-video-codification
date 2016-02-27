//
// Created by root on 12/2/15.
//

#include "RawIntraFrameCodecLossy.h"
#include "Golomb/Golomb.h"

using namespace std;

RawIntraFrameCodecLossy::RawIntraFrameCodecLossy(char *inputFilePath, char *outputFilePath) {
    this->inputFilePath = inputFilePath;
    this->outputFilepath = outputFilePath;
}

void RawIntraFrameCodecLossy::intraFrameEncode(int predictorMode, int q_y, int q_cr, int q_cb, int showVideo, int m) {
    ifstream ifs(this->inputFilePath);
    string line;
    getline(ifs, line);

    // parse line

    int frame_size_Ref_V, frame_size_Ref_H, frameRate;
    istringstream(line) >> frame_size_Ref_V;
    istringstream(line) >> frame_size_Ref_H;
    istringstream(line) >> frameRate;

    img = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC3);
    intermediate = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC3);

    predY = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC1);
    predU = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC1);
    predV = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8UC1);

    *predY = Scalar(0);
    *predU = Scalar(0);
    *predV = Scalar(0);

    int frameCount = 0;

    int x;
    GolombEncoder g(this->outputFilepath, m);

    Mat chan[3];

    yyMat = new Mat(Size(frame_size_Ref_V, frame_size_Ref_H), CV_8U);

    predU = resizeFrame(predU);
    predV = resizeFrame(predV);

    streampos begin,end;
    ifstream myfile (this->inputFilePath, ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    cout << "Original Size#: " << (end-begin) << " bytes.";

    cout << "\nStarting compression" << endl;
    cout << "Encoded frames#: ";

    while (ifs.read((char *) img->ptr(), frame_size_Ref_V * frame_size_Ref_H * 3)) {
        cvtColor(*img, *intermediate, CV_BGR2YCrCb);

        split(*intermediate, chan);

        *yyMat = chan[0];

        uuMat = resizeFrame(&chan[2]);
        vvMat = resizeFrame(&chan[1]);


        if(showVideo == 1) {
            imshow("YY component", *yyMat);
            imshow("UU recised component", *uuMat);
            imshow("VV recised component", *vvMat);
        }

        predictMat(predictorMode);

        if(showVideo == 1) {
            imshow("YY compressed component", *predY);
            imshow("UU recised compressed component", *predU);
            imshow("VV recised compressed component", *predV);
            waitKey(5);
        }

        for (int i = 0; i < predY->rows; i++) {
            for (int j = 0; j < predY->cols; j++) {
                x = (int) yyMat->at<uchar>(i, j) - (int) predY->at<uchar>(i, j);
                x = floor(x / q_y);
                g.WriteInt(abs(x));
                if (x != 0) {
                    if (x < 0)
                        g.WriteBit(1);

                    else
                        g.WriteBit(0);
                }

            }
        }


        for (int i = 0; i < predU->rows; i++) {
            for (int j = 0; j < predU->cols; j++) {
                x = (int) uuMat->at<uchar>(i, j) - (int) predU->at<uchar>(i, j);
                x = floor(x / q_cr);
                g.WriteInt(abs(x));
                if (x != 0) {
                    if (x < 0)
                        g.WriteBit(1);

                    else
                        g.WriteBit(0);

                }
            }
        }

        for (int i = 0; i < predV->rows; i++) {
            for (int j = 0; j < predV->cols; j++) {
                x = (int) vvMat->at<uchar>(i, j) - (int) predV->at<uchar>(i, j);
                x = floor(x / q_cb);
                g.WriteInt(abs(x));
                if (x != 0) {
                    if (x < 0)
                        g.WriteBit(1);

                    else
                        g.WriteBit(0);

                }
            }
        }

        cout << ++frameCount << " ";

    }
    g.Close();

    ofstream appendEnd;
    appendEnd.open(this->outputFilepath, ios::app);
    // here we write the header, at the end of the file
    frameCount--;
    appendEnd << "\n";
    appendEnd << to_string(m) << " ";
    appendEnd << to_string(q_y) << " ";
    appendEnd << to_string(q_cr) << " ";
    appendEnd << to_string(q_cb) << " ";
    appendEnd << to_string(frameCount) << " ";
    appendEnd << to_string(frame_size_Ref_V) << " ";
    appendEnd << to_string(frame_size_Ref_H) << " ";
    appendEnd << to_string(predictorMode) << " ";
    appendEnd << to_string(frameRate) << " ";
    appendEnd.close();
}

Mat *RawIntraFrameCodecLossy::resizeFrame(Mat *matToResize) {
    int pInit;

    resize = new Mat(matToResize->rows / 2, matToResize->cols / 2, CV_8UC1);

    for (int i = 0; i < matToResize->rows; i++) {
        for (int j = 0; j < matToResize->cols; j++) {
            pInit = (int) matToResize->at<uchar>(i, j);
            resize->at<uchar>(i/2, j/2) = pInit;
        }
    }
    matToResize->release();
    return resize;
}

void RawIntraFrameCodecLossy::predictMat(int predictorMode) {
    if (predictorMode == 1) {
        // a
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if (j == 0) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j - 1);

                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if (j == 0) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j - 1);

                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if (j == 0) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j - 1);

                }
            }
        }
    }
    else if (predictorMode == 2) {
        // b
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if (i == 0) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i - 1, j);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if (i == 0) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i - 1, j);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if (i == 0) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i - 1, j);
                }
            }
        }
    }
    else if (predictorMode == 3) {
        // c
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
    }
    else if (predictorMode == 4) {
        // a + b - c
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                } else {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j - 1) +
                            (int) yyMat->at<uchar>(i - 1, j) - (int) yyMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                } else {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j - 1) + (int) uuMat->at<uchar>(i - 1, j) -
                            (int) uuMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                } else {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j - 1) + (int) vvMat->at<uchar>(i - 1, j) -
                            (int) vvMat->at<uchar>(i - 1, j - 1);
                }
            }
        }
    }
    else if (predictorMode == 5) {
        // a + (b - c) / 2
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int)((int) yyMat->at<uchar>(i, j-1) + ((int) yyMat->at<uchar>(i-1, j) -
                            (int) yyMat->at<uchar>(i-1, j - 1))/2);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int)((int) uuMat->at<uchar>(i, j - 1) + ((int) uuMat->at<uchar>(i - 1, j) -
                            (int) uuMat->at<uchar>(i - 1, j - 1)) / 2);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int)((int) vvMat->at<uchar>(i, j - 1) + ((int) vvMat->at<uchar>(i - 1, j) -
                            (int) vvMat->at<uchar>(i - 1, j - 1)) / 2);
                }
            }
        }
    }
    else if (predictorMode == 6) {
        // b + (a - c) / 2
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predY->at<uchar>(i, j) = (int) yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int)(int) yyMat->at<uchar>(i - 1, j) + (((int) yyMat->at<uchar>(i, j - 1) -
                            (int) yyMat->at<uchar>(i - 1, j - 1)) / 2);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predU->at<uchar>(i, j) = uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int)(int) uuMat->at<uchar>(i - 1, j) + (((int) uuMat->at<uchar>(i, j - 1) -
                            (int) uuMat->at<uchar>(i - 1, j - 1)) / 2);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int)(int) vvMat->at<uchar>(i - 1, j) + (((int) vvMat->at<uchar>(i, j - 1) -
                            (int) vvMat->at<uchar>(i - 1, j - 1)) / 2);
                }
            }
        }
    }
    else if (predictorMode == 7) {
        // (a + b) / 2
        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predY->at<uchar>(i, j) = yyMat->at<uchar>(i, j);
                }
                else {
                    predY->at<uchar>(i, j) = (int)(((int) yyMat->at<uchar>(i, j - 1) + (int) yyMat->at<uchar>(i - 1, j)) / 2);
                }
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predU->at<uchar>(i, j) = (int) uuMat->at<uchar>(i, j);
                }
                else {
                    predU->at<uchar>(i, j) = (int) (((int) uuMat->at<uchar>(i, j - 1) + (int) uuMat->at<uchar>(i - 1, j)) / 2);
                }
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                if ((i == 0) && (j == 0)) {
                    predV->at<uchar>(i, j) = (int) vvMat->at<uchar>(i, j);
                }
                else {
                    predV->at<uchar>(i, j) = (int) (((int) vvMat->at<uchar>(i, j - 1) + (int) vvMat->at<uchar>(i - 1, j)) / 2);
                }
            }
        }
    }
    else if (predictorMode == 8) {
        uchar preditor;
        uchar min = 0;
        uchar max = 0;

        uchar a = 0;
        uchar b = 0;
        uchar c = 0;

        for (int i = 1; i < yyMat->rows; i++) {
            for (int j = 1; j < yyMat->cols; j++) {
                // YY
                a = yyMat->at<uchar>(i, j - 1);
                b = yyMat->at<uchar>(i - 1, j);
                c = yyMat->at<uchar>(i - 1, j - 1);

                if (a >= b){
                    max = a;
                    min = b;
                }
                else {
                    max = b;
                    min = a;
                }

                if (c >= max)
                    preditor = min;
                else if (c <= min)
                    preditor = max;
                else
                    preditor = a + b - c;

                predY->at<uchar>(i, j) = preditor;
            }
        }
        for (int i = 1; i < uuMat->rows; i++) {
            for (int j = 1; j < uuMat->cols; j++) {
                // UU

                a = uuMat->at<uchar>(i, j-1);
                b = uuMat->at<uchar>(i-1, j);
                c = uuMat->at<uchar>(i- 1, j-1);

                if (a > b)
                    max = a;
                else
                    max = b;

                if (b > a)
                    min = a;
                else
                    min = b;

                if (c >= max)
                    preditor = min;
                else if (c <= min)
                    preditor = max;
                else
                    preditor = a + b - c;

                predU->at<uchar>(i, j) = preditor;
            }
        }
        for (int i = 1; i < vvMat->rows; i++) {
            for (int j = 1; j < vvMat->cols; j++) {
                // VV
                a = vvMat->at<uchar>(i, j-1);
                b = vvMat->at<uchar>(i-1, j);
                c = vvMat->at<uchar>(i-1, j-1);

                if (a > b)
                    max = a;
                else
                    max = b;

                if (b > a)
                    min = a;
                else
                    min = b;

                if (c >= max)
                    preditor = min;
                else if (c <= min)
                    preditor = max;
                else
                    preditor = a + b - c;

                predV->at<uchar>(i, j) = preditor;
            }
        }
    }
    else {
        cout << "Bad compression mode" << endl;
        exit(-1);
    }
}

string getLastLine(ifstream& in)
{
    std::string line;
    while (in >> std::ws && std::getline(in, line)) // skip empty lines
        ;

    return line;
}

void RawIntraFrameCodecLossy::intraFrameDecode(int predictorMode, int showVideo) {
    GolombDecoder g(this->inputFilePath);
    ofstream output(this->outputFilepath,  ios::trunc | ios::binary);

    unsigned int m, q_y, q_cr, q_cb, frameCount, frameResolution_V, frameResolution_H, format, frameRate;

    streampos begin,end;
    ifstream myfile (this->inputFilePath, ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    cout << "\nCompressed Size#: " << (end-begin) << " bytes.\n";

    ifstream headerReader(this->inputFilePath);

    istringstream iss(getLastLine(headerReader));
    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};

    headerReader.close();

    m = (unsigned)stoul(tokens[0]);
    q_y = (unsigned)stoul(tokens[1]);
    q_cr = (unsigned)stoul(tokens[2]);
    q_cb = (unsigned)stoul(tokens[3]);
    frameCount = (unsigned)stoul(tokens[4]);
    frameResolution_V = (unsigned)stoul(tokens[5]);
    frameResolution_H = (unsigned)stoul(tokens[6]);
    format = (unsigned)stoul(tokens[7]);
    frameRate = (unsigned)stoul(tokens[8]);

    cout << endl;
    cout << "Parsed Header: " << "m# " << m << endl;
    cout << "               " << "q_y_#: " << q_y << endl;
    cout << "               " << "q_cr#: " << q_cr << endl;
    cout << "               " << "q_cb#: " << q_cb << endl;
    cout << "               " << "Frames#: " << frameCount << endl;
    cout << "               " << "Vertical Resolution#: " << frameResolution_V << endl;
    cout << "               " << "Horizontal Resolution#: " << frameResolution_H << endl;
    cout << "               " << "Encoding Format#: " << format << endl;
    cout << "               " << "Frame Rate#: " << frameRate << endl;

    g.SetParameter(m);      // sets m

    unsigned long long amountOfFrames = frameCount + 1;
    unsigned long long yy_IntCount = frameResolution_V*frameResolution_H;
    unsigned long long uu_vv_IntCount = (frameResolution_V/2) * (frameResolution_H/2);
    unsigned long long currentTotalIntCounter = 0;

    Mat* yyMat = new Mat(frameResolution_V, frameResolution_H, CV_8UC1);
    Mat* uuMat = new Mat(frameResolution_V/2, frameResolution_H/2, CV_8UC1);
    Mat* vvMat = new Mat(frameResolution_V/2, frameResolution_H/2, CV_8UC1);

    cout << "\nStarting decompression" << endl;
    cout << "Decoded frames#: ";
    int e;
    int x = 0;
    int z = 0;
    for(unsigned long long i = 0; i < amountOfFrames; i++){
        for(unsigned long long j = 0; j < yy_IntCount; j++){

            e = g.ReadInt();
            if(e != 0 && g.ReadBit())
                e *= -1;

            e = e * q_y;

            currentTotalIntCounter++;

            x = j / 1024;
            z = j % 1024;

            //yyTmpArray[j] = x;

            if(x == 0 || z == 0)
                yyMat->at<uchar>(x,z) = e;
            else {

                int predictor = predictValue(predictorMode, yyMat, x, z);
                yyMat->at<uchar>(x, z) = (int) predictor + e;
            }

        }
//        yyDecodedMat->data = yyData;
//        yyTmpMat->data = yyTmpArray;
//        for(unsigned long long j = 0; j < yyDecodedMat->rows; j++){
//            for(unsigned long long k = 0; k < yyDecodedMat->cols; k++){
//                yyMat->at<uchar>(j, k) = yyDecodedMat->at<uchar>(j, k) + yyTmpMat->at<uchar>(j, k);
//            }
//        }

        for(unsigned long long j = 0; j < uu_vv_IntCount; j++){
            e = g.ReadInt();
            if(e != 0 && g.ReadBit())
                e *= -1;

            e = e * q_cr;

            currentTotalIntCounter++;

            x = j / 512;
            z = j % 512;

            //yyTmpArray[j] = x;

            if(x == 0 || z == 0)
                uuMat->at<uchar>(x,z) = e;
            else {

                int predictor = predictValue(predictorMode, uuMat, x, z);
                uuMat->at<uchar>(x, z) = (int) predictor + e;
            }
        }

        for(unsigned long long j = 0; j < uu_vv_IntCount; j++) {
            e = g.ReadInt();
            if (e != 0 && g.ReadBit())
                e *= -1;

            e = e * q_cb;

            currentTotalIntCounter++;


            x = j / 512;
            z = j % 512;

            //yyTmpArray[j] = x;

            if (x == 0 || z == 0)
                vvMat->at<uchar>(x, z) = e;
            else {
                int predictor = predictValue(predictorMode, vvMat, x, z);
                vvMat->at<uchar>(x, z) = (int) predictor + e;
            }
//            if (currentTotalIntCounter > totalAmountOfInts + 1) {
//                cout << "Exceded total int counter" << endl;
//                exit(-1);
//            }
        }

        cout << i + 1 << " ";

        Mat ycrcb_merged(Size(1024, 1024), CV_8UC3);
        for(int i = 0 ; i < frameResolution_V ; i++){
            for(int j = 0 ; j < frameResolution_H ; j++){
                ycrcb_merged.at<Vec3b>(i,j).val[0] = (unsigned int) yyMat->at<uchar>(i, j);
                ycrcb_merged.at<Vec3b>(i,j).val[1] = (unsigned int )uuMat->at<uchar>(i/2, j/2);
                ycrcb_merged.at<Vec3b>(i,j).val[2] = (unsigned int) vvMat->at<uchar>(i/2, j/2);
            }

        }

        // Write video info
        int quinze =  15;

        output << frameResolution_H << ' ';
        output << frameResolution_V << ' ';
        output << quinze << ' ';
        output << "rgb" << '\n';

        cvtColor(ycrcb_merged, ycrcb_merged, CV_YCrCb2RGB);

//        output.write((char*) ycrcb_merged.ptr(), frameResolution_V * frameResolution_H * 3);


//        Vec3b* p;
//        Mat aux(ycrcb_merged.rows, ycrcb_merged.cols, CV_8UC3);
//        cvtColor(ycrcb_merged, aux, CV_YCrCb2RGB);
//
//        for (int row = 0; row < ycrcb_merged.rows; ++row) {
//            p = aux.ptr<Vec3b>(row);
//            for (int col = 0; col < ycrcb_merged.cols; ++col) {
//                output << p[col].val[0] << p[col].val[1] << p[col].val[2];
//            }
//        }

        Vec3b* p;
        for(int row = 0; row < ycrcb_merged.rows ; row++) {
            p = ycrcb_merged.ptr<Vec3b>(row);
            for(int col = 0 ; col < ycrcb_merged.cols ;  col++) {
                output << p[col].val[0];
                output << p[col].val[1];
                output << p[col].val[2];
                //output << ycrcb_merged.at<Vec3b>(row,col).val[0] << ycrcb_merged.at<Vec3b>(row,col).val[1] << ycrcb_merged.at<Vec3b>(row,col).val[2];
            }
        }

       cvtColor(ycrcb_merged, ycrcb_merged, CV_RGB2BGR);

        if(showVideo == 1) {
            imshow("Decompressed", ycrcb_merged);
            imshow("UU recised decompressed component", *yyMat);
            imshow("UU recised decompressed component", *uuMat);
            imshow("VV recised decompressed component", *vvMat);
            waitKey(5);
        }
    }

//    begin,end;
//    ifstream output (this->outputFilepath, ios::binary);
//    begin = output.tellg();
//    output.seekg (0, ios::end);
//    end = output.tellg();
//    output.close();
//    cout << "\nLossy Decompressed Size#: " << (end-begin) << " bytes.\n";


    g.Close();
output.close();
}

short RawIntraFrameCodecLossy::predictValue(int predictorMode, Mat *xxMat, int i, int j) {

    if (predictorMode == 1) {
        // a
        if (j == 0) {
            return (int) xxMat->at<uchar>(i, j);
        }
        else {
            return (int) xxMat->at<uchar>(i, j - 1);
        }
    }

    else if (predictorMode == 2) {
        // b
        if (i == 0) {
            return (int) xxMat->at<uchar>(i, j);
        }
        else {
            return (int) xxMat->at<uchar>(i - 1, j);
        }
    }
    else if (predictorMode == 3) {
        // c
        if ((i == 0) && (j == 0)) {
            return (int) xxMat->at<uchar>(i, j);
        }
        else {
            return (int) xxMat->at<uchar>(i - 1, j - 1);
        }
    }
    else if (predictorMode == 4) {
        // a + b - c
        if ((i == 0) && (j == 0)) {
            return (int) xxMat->at<uchar>(i, j);
        } else {
            return (int) xxMat->at<uchar>(i, j - 1) +
                    (int) xxMat->at<uchar>(i - 1, j) - (int) xxMat->at<uchar>(i - 1, j - 1);
        }
    }
    else if (predictorMode == 5) {
        // a + (b - c) / 2
        if ((i == 0) && (j == 0)) {
            return xxMat->at<uchar>(i, j);
        }
        else {
            return (int) ((int) xxMat->at<uchar>(i, j-1) + ((int) xxMat->at<uchar>(i-1, j) -
                    (int) xxMat->at<uchar>(i-1, j - 1))/2);
        }
    }
    else if (predictorMode == 6) {
        // b + (a - c) / 2
        if ((i == 0) && (j == 0)) {
            return (int) xxMat->at<uchar>(i, j);
        }
        else {
            return (int) xxMat->at<uchar>(i - 1, j) + (((int) xxMat->at<uchar>(i, j - 1) - (int) xxMat->at<uchar>(i - 1, j - 1)) / 2);
        }
    }
    else if (predictorMode == 7) {
        // (a + b) / 2
        if ((i == 0) && (j == 0)) {
            return (int) xxMat->at<uchar>(i, j);
        }
        else {
            return (int)(((int) xxMat->at<uchar>(i, j - 1) + (int) xxMat->at<uchar>(i - 1, j)) / 2);
        }
    }
    else if (predictorMode == 8) {
        uchar preditor;
        uchar min = 0;
        uchar max = 0;

        uchar a = 0;
        uchar b = 0;
        uchar c = 0;

        a = xxMat->at<uchar>(i, j - 1);
        b = xxMat->at<uchar>(i - 1, j);
        c = xxMat->at<uchar>(i - 1, j - 1);

        if (a >= b){
            max = a;
            min = b;
        }
        else {
            max = b;
            min = a;
        }

        if (c >= max)
            preditor = min;
        else if (c <= min)
            preditor = max;
        else
            preditor = a + b - c;

        return preditor;
    }

    else {
        cout << "Bad compression mode" << endl;
        exit(-1);
    }
}