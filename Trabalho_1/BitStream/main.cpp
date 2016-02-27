#include "BitStream.h"

using namespace std;

int main()
{
    cout << "BitStream, exercise 1_1" << endl;

    unsigned int bufferSize = 7;
    BitStream fileHandle("/Users/joelpinheiro/Documents/CAV1/cav1516-65151-42967/BitStream/test_file_1", bufferSize);
    //BitStream fileHandle("/root/ClionProjects/cav1516-65151-42967/BitStream/test_file_1", bufferSize);

    /*
     * Reading test
     * */
    fileHandle.openFileToRead();
    cout << "File " << fileHandle.getFileName() << " size is: "
    << fileHandle.getSize() << " bytes." << endl;

    fileHandle.readBit(0);      // read to buffer from the start, arbitrary

    cout << "Buffer contents: ";
    string buff = fileHandle.getBufferContents();
    for(unsigned int i = 0; i < bufferSize; i++) {
        cout << buff[i];
    }
    cout << endl;

    for(unsigned int i = 0; i < bufferSize; i++){
        for(unsigned int j = 0; j < 8; j++){
            cout << fileHandle.getBitToText(i, j);
        }
        cout << endl;
    }

    cout << endl;

    fileHandle.closeFileRead();

    /*
     * Writing test
     * */
    fileHandle.openFileToWrite(REPLACE_ONLY);
    char writeBuffer[] = {'T','e','s','t',' ',(char)0x89, 0b01010100, '\n', '\r', ' '};
    fileHandle.setBufferContents(writeBuffer);
    string wBuff = fileHandle.getBufferContents();
    cout << "Write buffer contents: ";
    for(unsigned int i = 0; i < sizeof(writeBuffer); i++) {
        cout << wBuff[i];
    }
    cout << "\n" << endl;
    fileHandle.writeBit(0, sizeof(writeBuffer));  // start on the beginning of the file overwriting the first byte, write 4 bytes from the buffer

    fileHandle.closeFileWrite();

    /*
     * Read and print the whole file
     * */
    fileHandle.openFileToRead();
        unsigned int fileSize = fileHandle.getSize();
        unsigned int bestBufferSize = fileHandle.getOptimalBufferSize();    // we can use a buffer sze of our choosing, or one sugested
        fileHandle.setBufferSize(bestBufferSize);
        cout << "Optimal Buffer Size: " << bestBufferSize << endl;
        string rBuff;

        cout << "File Contents:" << endl;

        for(unsigned int i = 0; i < fileSize; i+=bestBufferSize){    // we can use a buffer size of our choosing, or one sugested
            fileHandle.readBit(i);
            rBuff = fileHandle.getBufferContents();
            for(unsigned int j = 0; j < bestBufferSize; j++) {
                cout << rBuff[j];
            }
    }

    cout << endl;

    cout << "File size: " << fileHandle.getSize() << endl;

    fileHandle.closeFileRead();

    return 0;
}