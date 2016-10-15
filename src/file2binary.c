#include <stdio.h>
#include <stdlib.h>

int main(void) {
  unsigned char c;
  int j;
  
  while (1) {
    // Read a char from stdin
    c = fgetc(stdin);
    // If end of stream then exit
    if (feof(stdin))
      break;

    // Print the char in binary to screen
    for (j = 0; j < 8; j++)
      printf("%d", !!((c << j) & 0x80));
  }

  return 0;
}

