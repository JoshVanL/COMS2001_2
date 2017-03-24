#include "Philosopher.h"

bool pickup(int index, int n, int pnt) {

  int C[20]; 

  while(semaphore());
  share(SHARE_READ, pnt, C, 20);

  if(C[index] == 0 && C[(index+1)%n] == 0) {
      C[index] = 1;
      C[(index+1)%n] = 1;

      while(semaphore());
      share(SHARE_WRITE, pnt, C, 20);

      return true;
  }
  return false;
}

void putdown(int index, int n, int pnt) {

  int C[20]; 

  while(semaphore());
  share(SHARE_READ, pnt, C, 20); 

  C[index] = 0;
  C[(index+1)%n] = 0;

  while(semaphore());
  share(SHARE_WRITE, pnt, C, 20);
  
  return;
}

void think(int n) {

  int sleep = curr_timer() + 3*n +1;
  while(curr_timer() < sleep);

  return;
}

void eat(int n) {

  int sleep = curr_timer() + 2*n +1;
  while(curr_timer() < sleep);

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
  write(STDOUT_FILENO, c, sizeof(c));
  write(STDOUT_FILENO, " ", 2);

  int index = (int) atoi(a);

  n = atoi(b);
  int pnt = atoi(c);
  setPriority(10);
 
  while(semaphore());
  share(SHARE_READ, pnt, C, 20); 
 
  C[19] =1;

  setPriority(10);
  while(semaphore());
  share(SHARE_WRITE, pnt, C, 20); 

  while(C[0] ==1){
    setPriority(0);
    while(semaphore());
    share(SHARE_READ, pnt, C, 20); 
  }
  setPriority(0);

  write(STDOUT_FILENO, "go ", 3);

  bool flag = false;
  int k=0;
  char* m;
  char* o;
  int l =0;

  while(k<100) {
  for(int i=0; i<3; i++) m[i] = C[i] + '0';
    k++;
    write(STDOUT_FILENO, "think", 5);
    think(index);
    flag = pickup(index, n, pnt);

    if(flag) {
        write(STDOUT_FILENO, "eat", 3);
        eat(index);
        putdown(index, n, pnt);
        write(STDOUT_FILENO, "putdown ", 8);
    } else {
        write(STDOUT_FILENO, "cant -> ", 8);
    }
  }
  exit( EXIT_SUCCESS );
}
