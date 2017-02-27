#include "Factory.h"

extern void main_Philosopher();

void main_Factory() {
  char* C[16];

  for (int i = 0; i < 16; i++) {
      C[i] = 0;
  }

  for(int i =0; i < 16; i++) {
    write(SHARED_MEM, C, 16);
  }

  for(int i= 0; i< 1; i++) {
    fork();
    exec(main_Philosopher);
  }
    


}


    
    
