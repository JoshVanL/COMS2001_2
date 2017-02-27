#include "Philosopher.h"

char* C[16];
int i;

void think( uint32_t x ) {
  if ( !( x & 1 ) || ( x < 2 ) ) {
    return;
  }

  for( uint32_t d = 3; ( d * d ) <= x ; d += 2 ) {
    if( !( x % d ) ) {
      return;
    }
  }

  return;
}

void eat( uint32_t x) {
    think(x);
    return;
}

void pickup() {
  while(*C[i] == 1 || *C[(i+1)%16] == 1) read(SHARED_MEM, *C, 16);
  *C[i] = *C[i+1] = 1;
  write(SHARED_MEM, &C, 16);
  return;
}


void putdown() {
  *C[i] = *C[i+1] = 0;
  write(SHARED_MEM, &C, 16);
  return;
}


void main_Philosopher() {
  i = 1;
  read(SHARED_MEM, *C, 16);
  long r = (i*1238321);

  while(1) {
    think(r);
    pickup();
    eat(r);
    putdown;
  }


  return;
}
