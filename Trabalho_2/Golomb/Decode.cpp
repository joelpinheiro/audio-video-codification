/*
 *	Simple program to test Golomb decoding. It reads a file produced by the
 *	companion encoder and writes (to stdout) a file in comma-separated
 *	format (csv) containing integers (that might be signed). It takes one
 *	command-line parameter: the encoded file name ("file"). Beware: very
 *	little checking is done, i.e., it is not foolproof!
 */

#include <stdlib.h>
#include <iostream>
#include "Golomb.h"

using namespace std;

int main(int argc, char *argv[]) {
	int x, n;

	if(argc != 2) {
		cerr << "Usage: Decode file" << endl;
		return -1;
	}

	GolombDecoder g(argv[1]);

	g.SetParameter(atoi(g.ReadString().c_str())); // reads m from the file
	n = atoi(g.ReadString().c_str()); // reads the number of integers to decode

	for(int i = 0 ; i < n ; i++) {
		x = g.ReadInt();
		if(x != 0 && g.ReadBit())
			x *= -1;

		cout << x;
		if(i != n - 1)
			cout << ",";

	}

	cout << endl;
	g.Close();
}

