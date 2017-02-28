#include "Factory.h" 
#define num 5

extern void main_Philosopher();


void main_Factory(char* args[]) {
  char* C[num];
  char* arg[10];

  for (int i = 0; i < num; i++) {
      C[i] = 0;
  }

  for(int i =0; i < num; i++) {
    write(SHARED_MEM, *C, num);
  }

  for(int i= 0; i< num; i++) {
    *C[i] =1;
    write(SHARED_MEM, *C, num);
    fork();
    itoa(*arg, i);
    //exec(main_Philosopher, *arg);
  }
    


  exit( EXIT_SUCCESS );
}
 
