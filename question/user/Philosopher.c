#include "Philosopher.h"

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

bool pickup(int index, int n) {

  int C[20]; 

  while(semaphore_down());
  share(SHARE_READ, 0, C, 20);
  semaphore_up();

  if(C[index] == 0 && C[(index+1)%3] == 0) {
      C[index] = 1;
      C[(index+1)%3] = 1;

      while(semaphore_down());
      share(SHARE_WRITE, 0, C, 20);
      semaphore_up();

      return true;
  } else {
      write(STDOUT_FILENO, "cant - ", 7);
  }
  return false;
}

void putdown(int index, int n) {

  int C[20]; 

  while(semaphore_down());
  share(SHARE_READ, 0, C, 20); 
  semaphore_up();

  C[index] = 0;
  C[(index+1)%3] = 0;

  while(semaphore_down());
  share(SHARE_WRITE, 0, C, 20);
  semaphore_up();
  
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
  int n;
  int C[20];

  char* a = argv[0];
  char* b = argv[1];
  char* c = argv[2];
  char* d;

  write(STDOUT_FILENO, a, 2);
  write(STDOUT_FILENO, ":", 2);
  write(STDOUT_FILENO, b, sizeof(b));
  write(STDOUT_FILENO, " ", 2);

  int index = (int) atoi(a);

  n = atoi(b);
  int pnt = (int) atoi(c);
 
  while(semaphore_down());
  share(SHARE_READ, pnt, C, 20); 
  semaphore_up();
 
  C[19] =1;

  while(semaphore_down());
  share(SHARE_WRITE, pnt, C, 20); 
  semaphore_up();

  while(C[0] ==1){
    while(semaphore_down());
    share(SHARE_READ, pnt, C, 20); 
    semaphore_up();
  }

  write(STDOUT_FILENO, "go ", 3);

  bool flag = false;
  int k=0;
  char* m;

  while(k<100) {
  for(int i=0; i<3; i++) m[i] = C[i] + '0';
    k++;
    think();
    flag = pickup(index, n);
    if(flag) {
        write(STDOUT_FILENO, "eat", 3);
        eat();
        eat();
        putdown(index, n);
    } else {
        write(STDOUT_FILENO, "cant", 4);
    }
  }
  exit( EXIT_SUCCESS );
}
