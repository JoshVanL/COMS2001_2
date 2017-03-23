#include "Philosopher.h"

bool pickup(int index, int n) {

  int C[20]; 

  while(semaphore_down());
  share(SHARE_READ, 0, C, 20);
  semaphore_up();

  if(C[index] == 0 && C[(index+1)%5] == 0) {
      C[index] = 1;
      C[(index+1)%5] = 1;

      while(semaphore_down());
      share(SHARE_WRITE, 0, C, 20);
      semaphore_up();

      return true;
  }
  return false;
}

void putdown(int index, int n) {

  int C[20]; 

  while(semaphore_down());
  share(SHARE_READ, 0, C, 20); 
  semaphore_up();

  C[index] = 0;
  C[(index+1)%5] = 0;

  while(semaphore_down());
  share(SHARE_WRITE, 0, C, 20);
  semaphore_up();
  
  return;
}

void think() {

  int sleep = curr_timer() + 3;
  while(curr_timer() < sleep);

  return;
}

void eat() {

  int sleep = curr_timer() + 3;
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
  char* o;
  int l =0;

  while(k<100) {
  for(int i=0; i<3; i++) m[i] = C[i] + '0';
    k++;
    write(STDOUT_FILENO, "think", 5);
    think();
    flag = pickup(index, n);
    if(flag) {
        write(STDOUT_FILENO, "eat", 3);
        eat();
        eat();
        putdown(index, n);
    } else {
        write(STDOUT_FILENO, "cant -> ", 8);

        for(int i=0; i<n; i++) {
          m[i] = C[i] + '0';
        }
        write(STDOUT_FILENO, m, index-1);
        write(STDOUT_FILENO, "[", 1);
        m += index;
        l = 0;
        for(int i=index; i<n; i++){
            o[k] = m[i];
            l++;
        }
        write(STDOUT_FILENO, o, n-index);
        write(STDOUT_FILENO, "]", 1);
    }
  }
  exit( EXIT_SUCCESS );
}
