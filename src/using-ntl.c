#include <NTL/GF2X.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "debug.h"

using namespace std;
using namespace NTL;

/* Assumiamo di avere in input (stdin) le rappresentazioni dei
   numeri scritte nella forma che GF2X si aspetta, e gliele diamo in
   pasto e ci facciamo calcolare il GCD di tutte, che viene stampato
   in output.
   
   Il formato dell'input è:
   - CRC size (numero)
   - Per ogni osservazione si ha:
   --- Input dei dati (GF2X form)
   --- Input del CRC (GF2X form)
   
   Formato dell'output:
   - Per ogni fattore del polinomio GCD di grado uguale alla size:
   --- Fattore di grado fissato (GF2X form)
*/

// Per compilare utilizzare il comando:
// g++ src/using-ntl.c -o bin/using-ntl -O2 -I$HOME/sw/include -L$HOME/sw/lib -lntl -lgmp -lgf2x -lm -Wall

// Utilizzeremo i vector di GF2X per meglio tenerci i numeri in oggetto
int main (int argc, char *argv[]) {
  vector<GF2X>  points;
  int crc_width, i;
  string line;

  // --- READING CIN FOR DATA
  // Input the first line and check if it starts with '#'
  check(!getline(cin, line), "Error: first line not read. Gamma not found");
  if (line.at(0) == '#') {
    cout << line;
    check(!getline(cin, line), "Error: gamma not read in sencond line after #");
  }
  crc_width = stoi(line);
  // Then input content and crc lines
  while (getline(cin, line)) {
    istringstream icontent, icrc;
    GF2X contentpoly, crcpoly;
    
    // First line is content
    icontent.str(line);
    icontent >> contentpoly;

    // Second line is crc
    check(!getline(cin, line), "Error: expected crc line after content");
    icrc.str(line);
    icrc >> crcpoly;

    // -- BASIC COMPUTATION:
    GF2X result;
    result = LeftShift(contentpoly, crc_width) - crcpoly;
    points.push_back(result);
    cerr << "Point read and syzigy performed. Total degree: " << deg(result) << endl;
  }

  cerr << "All data read. Total points: " << points.size() << ". Now starting computation" << endl;
  // --- COMPUTATION
  GF2X gcd = GF2X::zero();
  for (i = 0; i < points.size(); i++) {
    gcd = GCD(gcd, points[i]);
    cerr << "GCD carried out. Current GCD degree: " << deg(gcd) << endl;
  }

  cerr << "End of computation. Now outputting final result" << endl;
  // --- DATA OUTPUT ON COUT
  cout << gcd << endl;
  return 0;
}

