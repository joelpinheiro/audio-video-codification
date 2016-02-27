/*
 *	Simple program to test Golomb encoding. It reads a file in comma-separated
 *	format (csv) containing integers (that might be signed) and writes (to
 *	stdout!) the Golomb encoded result. It takes two command-line parameters:
 *	the Golomb parameter ("m") and the csv file name ("file"). Beware: very
 *	little checking is done, i.e., it is not foolproof!
 */

#include <stdlib.h>
#include <iostream>
#include "Golomb.h"

using namespace std;

int main(int argc, char *argv[]) {
	int m, x;

	if(argc != 3) {
		cerr << "Usage: Encode m file" << endl;
		return -1;
	}

	m = atoi(argv[1]);
	ifstream inpFile(argv[2]);

	if(!inpFile.is_open()) {
		cerr << "Error: could not open file" << endl;
		return -1;
	}

	GolombEncoder g("/dev/stdout", m);

	// Count symbols to encode
	int n = 0;
	int c = ',';
	while(c == ',') {
		inpFile >> x;
		c = inpFile.get();
		n++;
	}

	inpFile.seekg(0);
	g.WriteString(to_string(m));
	g.WriteString(to_string(n));

	c = ',';
	while(c == ',') {
		inpFile >> x;
		c = inpFile.get();
		g.WriteInt(abs(x));
		if(x != 0) {
			if(x < 0)
				g.WriteBit(1);

			else
				g.WriteBit(0);

		}

	}

	g.Close();
}

