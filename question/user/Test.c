#include "Test.h"

void main_Test() {
  char* C[16];

  for (int i = 0; i < 16; i++) {
      *C[i] = 1;
  }

  write(SHARED_MEM, "hi", 2);
 

  return;
}


    
    
