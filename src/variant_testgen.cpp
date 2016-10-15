#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
using namespace std;

int main(int argc, char * argv[]) {
	if(argc < 4) {
		cerr << "Error: not enough arguments provided." << endl;
	}
	ofstream fout (argv[1]);
	int N=argv[2][0]-'0';
	int M=argv[3][0]-'0';
	srand(time(NULL));
	
	int i,j;
	for(j=0;j<M;j++) {
		for(i=0;i<32*N;i++) {
			fout << rand()%2;
		}
		fout <<endl;
	}
}
