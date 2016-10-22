#include <stdio.h>
#include <stdlib.h>
#include "f2p.c"

int main (void) {
  F2Polynomial A;

  f2p_init(A);
  f2p_read_stream(A, stdin);
  f2p_print(A);
  f2p_free(A);
  return 0;
}

