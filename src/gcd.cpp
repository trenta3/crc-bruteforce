#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>

#define BPI ((unsigned int)(sizeof(int)<<3))
// TODO: Questi dovranno essere sostituiti con le operazioni con gli shift e le mask
#define divBPI(N) ((N)/(BPI))
#define modBPI(N) ((N)%(BPI))
#define self (*this)

/* get MACRO
 * We first check if we are accessing past the degree of the polynomial. In that case we return 0.
 * Otherwise we go read the polynomial value at that place.
 */
#define get(P, N) (((P).deg <= (N)) ? 0 : (((P).poly[divBPI(N)] & (1 << modBPI(N))) >> modBPI(N)))

/* set MACRO
 * We first check if we are accessing past the degree of the polynomial. In that case we resize the polynomial vector.
 * Then we have the two cases of writing one or zero. In these cases we proceed adeguately.
 */
#define set(P, N, k) { if (divBPI((P).deg-1) < divBPI(N)) { ((P).poly.resize(divBPI(N)+1)); } \
    (k==0) ? ((P).poly[divBPI(N)] &= ~(unsigned int)(1 << modBPI(N))) : ((P).poly[divBPI(N)] |= (1 << modBPI(N))); }
 
using namespace std;

typedef unsigned int uint;

class F2poly {
	public:
		vector<uint> poly;
		int deg;
		
		F2poly(){
			deg=0;
		};
		
		F2poly(int d, vector<uint> V) {
			deg=d;
			poly=V;	
		};
		
		void read(string line) {
			poly.clear();
			deg=0;
			uint i;
			for(i=0;i<line.size();i++) {
				if(deg%BPI==0) {
					poly.push_back(0);
				}
				set(self, i, ((int)(line[i]-'0')));
				deg++;
			}
			return;
		}
		
		void print(ofstream& out) {
			int i;
			for(i=0;i<deg;i++) {
				out << get(self, i);
			}
			return;		
		}
		
		void print() {
			int i;
			for(i=0;i<deg;i++) {
				cout << get(self, i);
			}
			cout << endl;
			return;	
		
		}

		void syzigy(F2poly Q, int gamma) {
			int i;
			
			if(Q.deg==0) {
			  cerr << "Cannot syzigy by 0.\n";
			  return;
			}
			
			if (gamma >= 0) {
			  //				for(i=Q.deg+gamma-1;i>=gamma;i--) {
			  for (i = max(Q.deg+gamma, deg)-1; i >= gamma; i--) {

			    int tmp = get(self, i) ^ get(Q, (i-gamma));

			    // Se dobbiano scrivere un 1 troppo avanti ci mettiamo degli zeri prima
			    if (tmp == 1) {
			      int newdeg = max(deg, i+1), j;
			      for (j = newdeg - 1; j >= deg; j--)
				{ set(self, j, 0); }
			      deg = newdeg;
			    }
			    set(self, i, tmp);
			  }

			  // Aggiustiamo il grado del polinomio rimasto controllando quanti zeri abbiamo nei termini più alti
				for (i=deg-1; i>=0; i--)
					if (get(self, i) == 0)
						deg--;
					else break;
					
			}
			return;
		}
		
		void mod(F2poly Q) {

			if(Q.deg==0) {
			  cerr << "Cannot divide by 0.\n";
				return;
			}
			
			while (deg >= Q.deg) {
				syzigy(Q, deg-Q.deg);
			}
			return;
		}
		
		F2poly& operator=(const F2poly& P) {
		  deg = P.deg;
		  poly = P.poly;
			return *this;
		}
		
};



F2poly gcd(F2poly& P, F2poly& Q) {
			F2poly A = (P.deg>=Q.deg)?P:Q;
			F2poly B = (P.deg>=Q.deg)?Q:P;
			if(B.deg == 0) {
				return A;
			} else {
				A.mod(B);
				F2poly T = F2poly(gcd(A, B));
				return T;
			}
		}
		
int main(int argc, char * argv[]) {
	ifstream fin (argv[1]);
	ofstream fout (argv[2]);
	
	F2poly P = F2poly(), Q = F2poly(), R;
	string line;
	int gamma;
	vector<F2poly> CRC;
	vector<F2poly> firm;
	F2poly temp = F2poly();

	cerr << "Starting program. Reading data.\n";
	
	if(fin.is_open()) {
	  // La prima riga contiene il gamma
		getline(fin,line);
		gamma=atoi(line.c_str());
		// Ci aspettiamo che ci sia almeno un'altra riga
		getline(fin, line);
		while( ! fin.eof() ) {
			temp.read(line);
			firm.push_back(temp);

			getline(fin,line);
			temp.read(line);

			CRC.push_back(temp);
			CRC[CRC.size()-1].syzigy(firm[firm.size()-1],gamma);
			// Getting line for next loop
			getline(fin, line);
		}
	}

	cerr << "All data read. gamma: " << gamma << ", # of points: " << CRC.size() << endl; 
	cerr << "Now starting computation...\n";
	
	while( CRC.size() >= 2) {
	  F2poly tmp;
	  tmp = gcd(CRC[CRC.size()-1], CRC[CRC.size()-2]);
		CRC.pop_back();
		CRC.pop_back();
		CRC.push_back(tmp);
		cerr << "One GCD was computed. remaining polynomials: " << CRC.size() << endl;
	}
	
	
	if(fout.is_open() ) {
		CRC[0].print(fout);	
	}
	cerr << "End of program. Hope the result is right.\n";
	return 0;
}
