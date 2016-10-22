#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

typedef long int lint;
typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned char uchar;

#define bool int
#define true 1
#define false 0

#define sz_ulint sizeof(ulint)
//#define divUL(A) ((ulint)(A)/(sz_ulint))
//#define modUL(A) ((ulint)(A)%(sz_ulint))

#define sz_realloc_coeff 1.5

/* On each function we have the attributes @attr
 * spbit: spurious bits safe
 * input: can input data
 * nomod: not modifying inputs
 */

typedef struct F2Polynomial {
  ulint* poly;  // Vector containing numbers
  size_t size;   // Currently allocate size (in units)
  ulint deg;    // Degree of the polynomial
} F2Polynomial;

inline ulint max (ulint a, ulint b) {
return (a >= b) ? a : b;
}

inline ulint min (ulint a, ulint b) {
return (a >= b) ? b : a;
}

inline ulint divUL (ulint d) {
return ((ulint)(d)/(sz_ulint));
}

inline ulint modUL (ulint d) {
return ((ulint)(d)%(sz_ulint));
}

// Function to free out the space occupied by polynomial
void f2p_free(F2Polynomial& P) {
  P.size = 0;
  P.deg = 0;
  free(P.poly);
}

// Function to init the polynomial and reserve space for it
void f2p_init(F2Polynomial& P) {
  P.poly = (ulint*) malloc(8*sz_ulint);
  check(P.poly == NULL, "in function f2p_init: calloc unable to allocate space. Aborting");
  P.size = 8;
  P.deg = 0;
}

// Function to resize the polynomial to hold at least a degree
inline void f2p_resize_to_hold(F2Polynomial& P, ulint degree) {
  if (divUL(degree) >= P.size) {
    ulint newsize = max(P.size * sz_realloc_coeff, degree);
    f2p_resize(P, newsize);
    P.size = newsize;
  }
}

// Function to copy a polynomial to another one
// Assume spurious bit safe
void f2p_copy(F2Polynomial& P, F2Polynomial& Q) {
  f2p_resize_to_hold(P, Q.deg);
  
  for (i = 0; i <= divUL(Q.deg); i++)
    P.poly[i] = Q.poly[i];
}

// Function to reallocate space for the polynomial
inline void f2p_resize(F2Polynomial& P, size_t space) {
  // ACHTUNG: The new bits are NOT set to zero!
  ulint* tmp = (ulint*) realloc(P.poly, sz_ulint*space);
  check(tmp == NULL, "in function f2p_resize: realloc unable to allocate space. Aborting");
  P.poly = tmp;
  P.size = space;
}

// DANGEROUS FUNCTION: does NOT set preceding bits to zero
inline void f2p_set_bit(F2Polynomial& P, ulint position, int bit) {
  if (bit == 0)
    P.poly[divUL(position)] &= ~(ulint)(1 << modUL(position));
  else if (bit == 1)
    P.poly[divUL(position)] |= (ulint)(1 << modUL(position));
  else
    check(true, "in function f2p_set_bit: unconceivable value for bit (%d) to set at position %lu for polynomial (%p)", bit, position, (void *)&P);
}

inline int f2p_get_bit(F2Polynomial& P, ulint position) {
  return (P.poly[divUL(position)] & (ulint)(1 << modUL(position))) >> modUL(position);
}

// Auxiliary function for reading strings and file handles
inline bool f2p_continue_reading(F2Polynomial& P, const char *string, ulint numchar) {
  ulint i;
  if (numchar == 0)
    numchar--;
  for (i=0; string[i] != '\0' && string[i] != '\n' && (numchar--) > 0; i++) {
    if (divUL(P.deg + i) >= (P.size))
      f2p_resize(P, P.size*sz_realloc_coeff);
    f2p_set_bit(P, P.deg + i, (string[i] - '0'));
  }
  P.deg += i;
  if (string[i] == '\n')
    return true;
  else return false;
}

// Read a 0 or 1 string and put it in the polynomial
// The string must be all zeros and ones (in ASCII)
// Probably slow
inline void f2p_read_string(F2Polynomial& P, const char *string) {
  P.deg = 0;
  f2p_continue_reading(P, string, 0);
}

/* void f2p_calculate_degree(F2Polynomial P) { */
/*   */  
/* } */

inline void f2p_zero_spurious_bits(F2Polynomial& P) {
  P.poly[divUL(P.deg)] &= ((1U << modUL(P.deg)) - 1);
}

#define BUFFER_SIZE 1025
// Read a number from a stream (file handle)
// @attr: spbit, input
void f2p_read_stream(F2Polynomial& P, FILE *fin) {
  char buf[BUFFER_SIZE];
  P.deg = 0;
  check(fin == NULL, "in function f2p_read_stream: file handle passed is NULL");
  while (fgets(buf, sizeof(char)*BUFFER_SIZE, fin) != NULL) {
    if (f2p_continue_reading(P, buf, sizeof(char)*BUFFER_SIZE))
      break;
  }
  // And then we set the boundaries appropiately (zeroing out bigger bits)
  f2p_zero_spurious_bits(P);
}

// Function to print the polynomial
// @attr: unmod, slow
void f2p_print(F2Polynomial& P) {
  ulint i;
  for (i = 0; i < P.deg; i++)
    printf("%d", f2p_get_bit(P, i));
  printf("\n");
}

// Function to and two polynomials
// modify P, unmod Q
void f2p_and(F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(P, Q.deg);

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);
  
  for (i = 0; i <= divUL(mindeg); i++)
    P.poly[i] &= Q.poly[i];
  for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
    P.poly[i] = 0U;

  P.deg = maxdeg;
}

// Function to and two polynomials and store in third
void f2p_and_st(F2Polynomial& R, F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(R, max(P.deg, Q.deg));

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);
  
  for (i = 0; i <= divUL(mindeg); i++)
    R.poly[i] = P.poly[i] & Q.poly[i];
  for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
    R.poly[i] = 0U;

  R.deg = maxdeg;
}

// Function to ior two polynomials
void f2p_ior(F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(P, Q.deg);

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);

  for (i = 0; i <= divUL(mindeg); i++)
    P.poly[i] |= Q.poly[i];
  if (P.deg <= Q.deg)
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      P.poly[i] = Q.poly[i];
  P.deg = maxdeg;
}

void f2p_ior_st(F2Polynomial& R, F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(R, max(P.deg, Q.deg));

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);

  for (i = 0; i <= divUL(mindeg); i++)
    R.poly[i] = P.poly[i] | Q.poly[i];
  if (P.deg <= Q.deg)
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      R.poly[i] = Q.poly[i];
  else
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      R.poly[i] = P.poly[i];
  R.deg = maxdeg;
}

// Function to xor two polynomials
void f2p_xor(F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(P, Q.deg);

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);

  for (i = 0; i <= divUL(mindeg); i++)
    P.poly[i] ^= Q.poly[i];
  if (P.deg <= Q.deg)
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      P.poly[i] = Q.poly[i];
  P.deg = maxdeg;
}

void f2p_xor_st(F2Polynomial& R, F2Polynomial& P, F2Polynomial& Q) {
  ulint i, mindeg, maxdeg;

  f2p_resize_to_hold(R, max(P.deg, Q.deg));

  mindeg = min(P.deg, Q.deg);
  maxdeg = max(P.deg, Q.deg);

  for (i = 0; i <= divUL(mindeg); i++)
    R.poly[i] = P.poly[i] ^ Q.poly[i];
  if (P.deg <= Q.deg)
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      R.poly[i] = Q.poly[i];
  else
    for (i = divUL(mindeg) + 1; i <= divUL(maxdeg); i++)
      R.poly[i] = P.poly[i];
  R.deg = maxdeg;
}

// Function to bitwise invert one polynomial
void f2p_not(F2Polynomial& P) {
  ulint i;

  for (i = 0; i <= divUL(P.deg); i++)
    P.poly[i] = ~ P.poly[i];
}

void f2p_not_st(F2Polynomial& R, F2Polynomial& P) {
  ulint i;

  f2p_resize_to_hold(R, P.deg);

  for (i = 0; i <= divUL(P.deg); i++)
    R.poly[i] = ~ P.poly[i];
}

// Function to calculate "long" division between polynomials
// modify Q R, unmod A B
void f2p_euclidean_division(F2Polynomial& A, F2Polynomial& B, F2Polynomial& Q, F2Polynomial& R) {
  
}


