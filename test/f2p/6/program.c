#include <stdio.h>
#include <stdlib.h>
#include "f2p.c"

int main (void) {
  F2Polynomial A, B;

  f2p_init(A);
  f2p_init(B);
  
  f2p_read_stream(A, stdin);
  f2p_read_stream(B, stdin);

  f2p_and(A, B);
  f2p_print(A);

  f2p_free(A);
  f2p_free(B);
  return 0;
}

