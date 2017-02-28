#include "Test.h"

void main_Test() {
  char C[16];
  char* O[2];
  *O = "BB";

  for (int i = 0; i < 16; i++) {
      C[i] = 1;
  }

  write(SHARED_MEM, *O, 2);
  read(SHARED_MEM, *O, 2);
  write( STDOUT_FILENO, *O, 2 );
 

  exit( EXIT_SUCCESS );
}


    
    
