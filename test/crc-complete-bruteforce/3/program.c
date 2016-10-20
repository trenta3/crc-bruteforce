#include <stdio.h>
#include <stdlib.h>
#include "crc-complete-bruteforce.c"

/* First test case for crc-complete-bruteforce
 * We check if we are able to read a polynomial from a file and to
 * write it out equal.
 */

int main (void) {
  F2Polynomial A;

  f2p_init(A);
  f2p_read_stream(A, stdin);
  f2p_print(A);
  f2p_free(A);
  return 0;
}

