#include "Factory.h" 

extern void main_Philosopher();

void main_Factory(int argc, char* argv[]) {

  void* addr = &main_Philosopher;
  int n = atoi(argv[0]);
  char nc[2];
  nc[0] = argv[0][0];
  nc[1] = argv[0][1];
  //int n =5;
  int C[20];
  char* arg[5] = {"0", nc, "0"};

  char p[16];
  int pnt = share_init();
  write(STDOUT_FILENO, nc, 2);

  if(n>0 && n < 20) {
  
    for(int i=0; i<20; i++) C[i]=0;
    C[0] = 1;
  
    while(semaphore()) write(STDOUT_FILENO, ">> ", 3);
    share(SHARE_WRITE, pnt, C, 20);

    for(int i=0; i<n; i++) {
      setPriority(10);

      if( i < 10) arg[0][0] = i + '0';
      else {
          arg[0][0] = '1';
          arg[0][1] = (i - 10) + '0';
      }

      pid_t pid  = fork();
      exec(addr, arg);

      while(C[19] ==0){
        setPriority(10);
        while(semaphore());
        share(SHARE_READ, pnt, C, 20); 
      }
      setPriority(10);

      C[19]=0;

      while(semaphore());
      share(SHARE_WRITE, pnt, C, 20);
      setPriority(10);

      write(STDOUT_FILENO, ">> ", 3);
    }
 
    C[0] = 0;

    while(semaphore());
    share(SHARE_WRITE, pnt, C, 20);
    setPriority(10);

    int sleep = 0;

    while(true) {
      sleep = curr_timer() + 10;
      while(curr_timer() < sleep);

      while(semaphore());
      share(SHARE_READ, 1, C, 20);

      for(int i=0; i<n; i++) {
        p[i] = C[i] + '0';
      }
      write(STDOUT_FILENO, p, n);
    }
  
  }


  exit( EXIT_SUCCESS );
}
 
