#include "Test.h"

void main_Test(int argc, char* argv[]) {

  write(STDOUT_FILENO, "1", 1);
  int C[20];

  for(int i=0; i<20; i++) C[i] = 0;

  while(semaphore_down());
  share(SHARE_WRITE, 0, C, 20);
  semaphore_up();


  write(STDOUT_FILENO, "2", 1);

  exit( EXIT_SUCCESS );
}


    
    
