#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>

#define D 6
using namespace std;

typedef unsigned int uint;

int _BIT_=0;
//int TESTING=2;

void polyprint( string s, ofstream& out) {
	uint i; int counter=0;
	for(i=0;i<s.size();i++) {
		if(s[i] == '1') { 
			if(counter>0) {
				out << "+";
			}
			out<<"x^";
			out << i;
			counter++;
		} else if(s[i] == '0') {
		
		} else {
			fprintf(stderr, "Error: impossible to polyprint a non-bit string.\n");
		
		}
	}	
	return;
}

void bitprint( string s, ofstream& out) {
	out << s;
	return;
}
 
void machine ( int par, string& in, string& out ) {
	uint i;
	//if(par != TESTING) return;
	//applico la trasformazione	
	switch(par) {
	
		case 1:
			out.assign(in);
			_BIT_=1;
			break;
		
		case 2:
			out.clear();
			out.resize(in.size());
			uint pos;
			if (_BIT_ == 1 ) {
				pos=0;
				while(pos+32 <= in.size() ) {
					out.replace(pos,8,in,pos+24,8);
					out.replace(pos+8,8,in,pos+16,8);
					out.replace(pos+16,8,in,pos+8,8);
					out.replace(pos+24,8,in,pos,8);
					pos+=32;
				} 
			} else if( _BIT_ == 0) {
				pos=0;
				while(pos+16 <= in.size() ) {
					out.replace(pos,8,in,pos+8,8);
					out.replace(pos+8,8,in,pos,8);
					pos+=16;
				}
			}
			break;
	
		case 3:
			out.assign(in);
			out.erase(32,16);
			break;
	
		case 4:
			out.assign(in);
			out.erase(32,32);
			break;
	
		case 5:
			out.assign(in);
			reverse(out.begin(), out.begin()+31);
			reverse(out.begin()+32,out.end());
			break;
		
		case 6:
			out.assign(in);
			for(i=0; i<32; i++) {
				if(in.at(i) == '1') {
					out.at(i)='0';
				} else if( in.at(i) == '0' ) {
					out.at(i)='1';
				} else {
					fprintf(stderr, "Error: not-bit character in file.\n");
				}
			}
			break;

		default:
			fprintf(stderr,"Error: unspecified parameter for machine.\n");
			break;
	}
			
	return;
}

int main(int argc, char* argv[] ) {
	if(argc < 3) {
		fprintf(stderr, "Error: %d arguments provided, 2 expected. Correct usage:\n %s nomeinput nomeoutput\n", argc-1, argv[0]);
	}
	ifstream fin (argv[1]);
	ofstream fout (argv[2]);
	vector< string > inputString;
	vector< string > outputString;
	
	//lettura input
	
	string line;
	if (fin.is_open() ) {
		while( getline(fin, line) ) {
			inputString.push_back(line);
			outputString.push_back(line);
		}
	} else {
		cout << "Error: impossible to open input file.\n";	
	}
	
	outputString.resize(inputString.size());
	// scrittura output
	
	if( fout.is_open() ) {
		int option=0; int j; uint i;
		int check= 1 << D; string oldtemp; string newtemp;
		int mask;
		while ( (option & check) == 0) {
			for(i=0;i<inputString.size();i++) {
				mask=1;
				oldtemp.assign(inputString[i]);
				newtemp.assign(inputString[i]);
				//pulisco le condizioni iniziali
				_BIT_=0;
				// faccio le modifiche indicate dall'option
				//cout << "OPTION " << option << endl;
				for(j=0;j<D;j++) {
					//cout << oldtemp << endl;
					if((mask&option) > 0) {
						machine(j+1,oldtemp,newtemp);
					}
					mask=mask<<1;
					oldtemp.assign(newtemp);
				}
				outputString[i].assign(newtemp);
			}
			//stampo outputString sul file
			//printf("option %d, control %d\n", option, (1<<(TESTING-1)));
			//if(option ==(1 << (TESTING-1)) || (option == 0) ) {
				for(i=0;i<outputString.size();i++) {
					//stampa la stringa
					//printf("Printing string\n");
					//fout << "printing string" << endl;
					bitprint(outputString[i].substr(32, outputString[i].size()-32), fout);
					fout <<endl;
					//stampa il CRC
					//printf("Printing CRC\n");
					//fout << "printing CRC" << endl;
					bitprint(outputString[i].substr(0,32), fout);
					fout<<endl<<"#"<<endl;			
				}
			//}
			option++;			
		}
		
	} else {
		cout << "Error: impossible to open output file.\n";
	
	}


	return 0;
}
