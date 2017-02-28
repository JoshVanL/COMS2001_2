#include "Philosopher.h"

#define num 5

char* C[num];
int i=0;

void think( uint32_t x ) {
  write(STDOUT_FILENO, "Thinking\n", 9);
  for(int c= 0; c < 999999*i; c++) {
      for(int d=0; d< 99999*i; c++) {
      }
  }
  return;
}

void eat( uint32_t x) {
  write(STDOUT_FILENO, "Eating\n", 7);
  for(int c= 0; c < 999999*i; c++) {
      for(int d=0; d< 99999*i; c++) {
      }
  }
  return;
}

void pickup() {
  while(*C[i] == 1 || *C[(i+1)%num] == 1){
      read(SHARED_MEM, *C, num);
      write(STDOUT_FILENO, "Waiting\n", 8);
  }
  *C[i] = *C[i+1] = 1;
  write(SHARED_MEM, *C, num);
  return;
}


void putdown() {
  *C[i] = *C[i+1] = 0;
  write(SHARED_MEM, *C, num);
  return;
}

void main_Philosopher() {

  read(SHARED_MEM, *C, num);
  int n =0;
  while(n<num) {
    if(*C[n] == 1){
      i =n;
      break;
    }
    n++;
  }


  long r = (i*1238321);

  while(n< 10) {
    think(r);
    pickup();
    eat(r);
    putdown;
    n++;
  }


  exit( EXIT_SUCCESS );
}
