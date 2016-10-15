#include <stdio>
#include <stdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <time>
using namespace std;

int main(int argc, char * argv[]) {
	if(argc < 4) {
		cerr << "Error: not enough arguments provided." << endl;
	}
	ofstream fout (argv[1]);
	unsigned int N=(int)(argv[2][0]);
	unsigned int M=(int)(argv[3][0]);
	srand(time(NULL));
	
	unsigned int i,j;
	for(j=0;j<M;j++) {
		for(i=0;i<32*N;i++) {
			fout << rand()%2;
		}
		fout <<endl;
	}
}
