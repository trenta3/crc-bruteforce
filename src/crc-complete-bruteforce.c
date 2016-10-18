#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

typedef long int lint;
typedef unsigned int uint;
typedef unsigned long int ulint;

#define bool int
#define true 1
#define false 0

#define sz_ulint sizeof(ulint)
#define divUL(A) ((ulint)(A)/(sz_ulint))
#define modUL(A) ((ulint)(A)%(sz_ulint))

#define sz_realloc_coeff 2

typedef struct F2Polynomial {
  ulint* poly;  // Vector containing numbers
  size_t size;   // Currently allocate size (in units)
  ulint deg;    // Degree of the polynomial
} F2Polynomial;

inline ulint max (ulint a, ulint b) {
  return (a >= b) ? a : b;
}

// Free the space used by the polynomial
void f2p_free(int s __attribute__ ((unused)), void* P) {
  F2Polynomial* Q = (F2Polynomial*) P;
  (*Q).deg = 0;
  (*Q).size = 0;
  free((*Q).poly);
}

// Function to init the polynomial and reserve space for it
bool f2p_init(F2Polynomial P) {
  P.poly = calloc(8, sz_ulint);
  check(P.poly == NULL, "in function f2p_init: calloc unable to allocate space. Aborting");
  on_exit(f2p_free, (void*)&P);
  P.size = 8;
  P.deg = 0;
}

// Function to reallocate space for the polynomial
bool f2p_resize(F2Polynomial P, size_t space) {
  // ACHTUNG: The new bits are NOT set to zero!
  ulint* tmp = realloc(P.poly, sz_ulint*space);
  check(tmp == NULL, "in function f2p_resize: realloc unable to allocate space. Aborting");
  P.poly = tmp;
  P.size = space;
}

// DANGEROUS FUNCTION: does NOT set preceding bits to zero
inline void f2p_set_bit(F2Polynomial P, ulint position, int bit) {
  if (bit == 0)
    P.poly[divUL(position)] &= ~(ulint)(1 << modUL(position));
  else if (bit == 1)
    P.poly[divUL(position)] |= (ulint)(1 << modUL(position));
  else
    check(true, "in function f2p_set_bit: unconceivable value for bit (%d) to set at position %ul for polynomial (%p)", bit, position, (void *)&P);
}

inline int f2p_get_bit(F2Polynomial P, ulint position) {
  return (P.poly[divUL(position)] & (ulint)(1 << modUL(position))) >> modUL(position);
}

// Auxiliary function for reading strings and file handles
void f2p_continue_reading(F2Polynomial P, const char *string, ulint numchar) {
  ulint i;
  for (i=0; string[i] != '\0' && (--numchar) >= 0; i++) {
    if (divUL(P.deg + i) >= P.size)
      f2p_resize(P, P.size*sz_realloc_coeff);
    f2p_set_bit(P, P.deg + i, (string[i] - '0'));
  }
  P.deg += i;
}

// Read a 0 or 1 string and put it in the polynomial
// The string must be all zeros and ones (in ASCII)
// Probably slow
inline void f2p_read_string(F2Polynomial P, const char *string) {
  P.deg = 0;
  f2p_continue_reading(P, string, 0);
}

void f2p_calculate_degree(F2Polynomial P) {
  
}

// Read a number from a stream (file handle)
void f2p_read_stream(F2Polynomial P, FILE *fin) {
  ulint i; char buf[1024]; size_t nread;
  P.deg = 0;
  check(fin == NULL, "in function f2p_read_stream: file handle passed is NULL");
  while ((nread = fread(buf, 1, sizeof(buf), fin)) > 0)
    f2p_continue_reading(P, buf, 1024);
}

// Function to print the polynomial
// Probably slow
void f2p_print(F2Polynomial P) {
  ulint i;
  for (i = 0; i < P.deg; i++)
    printf("%d", f2p_get_bit(P, i));
  printf("\n");
}

// Function to and two polynomials
void f2p_and(F2Polynomial P, F2Polynomial Q) {
  ulint i;
  if (divUL(Q.deg) >= P.size)
    f2p_resize(P, P.size*sz_realloc_coeff);

  // TODO: Aumentare efficienza nel caso in cui Q è più corto di P  
  for (i = 0; i < divUL(P.deg); i++)
    P.poly[i] &= Q.poly[i];
  P.poly[i] &= (1U << modUL(P.deg) - 1) & Q.poly[i];
  for (i = divUL(P.deg) + 1; i < divUL(Q.deg); i++)
    P.poly[i] = Q.poly[i];
  P.poly[i] = Q.poly[i] & (1U << modUL(Q.deg) - 1);
  P.deg = max(P.deg, Q.deg);
}


