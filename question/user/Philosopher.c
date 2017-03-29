#include "Philosopher.h"

bool pickup(int index, int n, int pnt) {

  int C[20]; 
  char p[5];

  share(SHARE_READ, pnt, C, 21);
  setPriority(100);
  while(semaphore());
 // for(int i=0; i<n; i++) p[i] = C[i] + '0';
  p[0] = index + '0';
  //write(STDOUT_FILENO, p, n);


  if(C[index] == 0 && C[(index+1) % n] == 0) {
      C[index] = 1;
      C[(index+1) % n] = 1;
      share(SHARE_WRITE, pnt, C, 20);
      return true;
  }

  share(SHARE_WRITE, pnt, C, 20);
  return false;
}

void putdown(int index, int n, int pnt) {

  int C[20]; 

  share(SHARE_READ, pnt, C, 20); 
  while(semaphore());

  C[index] = 0;
  C[(index+1)%n] = 0;

  share(SHARE_WRITE, pnt, C, 20);
  
  return;
}

void think(int n) {

  int sleep = curr_timer() + 3*n +6;
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

  char a[2];
  char b[2];
  char c[2]; 
  a[0]  = argv[0][0];
  a[1]  = argv[0][1];
  b[0]  = argv[1][0];
  b[1]  = argv[1][1];
  c[0]  = argv[2][0];
  c[1]  = argv[2][1];
  char* d;

  write(STDOUT_FILENO, a, 2);
  write(STDOUT_FILENO, ":", 1);
  write(STDOUT_FILENO, b, sizeof(b));
  write(STDOUT_FILENO, " ", 1);
  //write(STDOUT_FILENO, c, sizeof(c));
  //write(STDOUT_FILENO, " ", 2);

  int index = (int) atoi(a);

  n = atoi(b);
  int pnt = atoi(c);
  setPriority(10);
 
  while(semaphore());
  share(SHARE_READ, pnt, C, 20); 
  setPriority(10);
 
  C[19] =1;

  while(semaphore());
  share(SHARE_WRITE, pnt, C, 20); 

  while(C[0] ==1){
    setPriority(-5);
    while(semaphore());
    share(SHARE_READ, pnt, C, 20); 
  }
  setPriority(0);

  write(STDOUT_FILENO, "go ", 3);

  int k=0;
  char* m;
  char* o;
  int l =0;

  while(k<100) {
  for(int i=0; i<3; i++) m[i] = C[i] + '0';
    k++;
    write(STDOUT_FILENO, "think", 5);
    think(index);
    while(semaphore());
    bool flag = pickup(index, n, pnt);

    if(flag) {
        write(STDOUT_FILENO, "eat", 3);
        eat(index);
        while(semaphore());
        putdown(index, n, pnt);
        write(STDOUT_FILENO, "putdown ", 8);
    } else {
        write(STDOUT_FILENO, "can't -> ", 9);
        setPriority(5);
    }
  }
  exit( EXIT_SUCCESS );
}
