#include "Philosopher.h"

int i;
int n;
int C[20];

int p( uint32_t x ) {
  if ( !( x & 1 ) || ( x < 2 ) ) {
    return ( x == 2 );
  }

  for( uint32_t d = 3; ( d * d ) <= x ; d += 2 ) {
    if( !( x % d ) ) {
      return 0;
    }
  }

  return 1;
}

int pickup() {
  share(SHARE_READ, 0, C, 20);
  if(C[i] == 0 && C[(i+1)%20] == 0) {
      C[i] = 1;
      C[(i+i)%n] = 1;
      share(SHARE_WRITE, 0, C, 20);
      return 1;
  }
  return 0;
}

void putdown() {
  share(SHARE_READ, 0, C, 20);
  C[i] = 0;
  C[(i+1)%n] = 0;
  share(SHARE_WRITE, 0, C, 20);
  return;
}

void think() {
  int m = 0;
    uint32_t lo = 1 <<  8;
    uint32_t hi = 1 << 16;

    for( uint32_t x = lo; x < hi; x++ ) {
      int r = p( x ); 
    }
  return;
}

void eat() {
    uint32_t lo = 1 <<  8;
    uint32_t hi = 1 << 16;

    for( uint32_t x = lo; x < hi; x++ ) {
      int r = p( x ); 
    }
  return;
}

void main_Philosopher(int argc, char* argv[]) {
  char* a = argv[0];
  char* b = argv[1];
  char* c = argv[2];
  write(STDOUT_FILENO, a, 2);
  write(STDOUT_FILENO, ":", 2);
  write(STDOUT_FILENO, b, sizeof(b));
  i = atoi(a);
  n = atoi(b);
  int pnt = (int) atoi(c);
 
  share(SHARE_READ, pnt, C, 20); 
 
  C[19] =1;
  share(SHARE_WRITE, pnt, C, 20); 
 
  write(STDOUT_FILENO, "think", 5);
  int k=0;
  while(k<100) {
    k++;
    think();
    think();
    think();
    think();
    think();
    if(pickup) {
        write(STDOUT_FILENO, "eat", 3);
        eat();
        eat();
        eat();
        eat();
        putdown();
        write(STDOUT_FILENO, "down - ", 7);
        write(STDOUT_FILENO, "think", 5);
    } else {
        write(STDOUT_FILENO, "cant - ", 7);
    }
  }
  exit( EXIT_SUCCESS );
}
