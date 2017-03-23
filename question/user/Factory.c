#include "Factory.h" 

extern void main_Philosopher();


void main_Factory(int argc, char* argv[]) {

  void* addr = &main_Philosopher;
  int n = atoi(argv[0]);
  int C[20];
  char* arg[5];
  arg[0] = "0";
  arg[1] = argv[0];
  char* p;

  if(n>0 && n < 20) {
  
    for(int i=0; i<20; i++) C[i]=0;
    C[0] = 1;

  
    int pnt = share_init();
    itoa(arg[2], pnt);
  
    while(semaphore_down()) write(STDOUT_FILENO, ">> ", 3);
    share(SHARE_WRITE, pnt, C, 20);
    semaphore_up();

    for(int i=0; i<n; i++) {
      fork();
      exec(addr, arg);

      while(C[19] ==0){
        while(semaphore_down());
        share(SHARE_READ, pnt, C, 20); 
        semaphore_up();
      }

      C[19]=0;
      while(semaphore_down());
      share(SHARE_WRITE, pnt, C, 20);
      semaphore_up();
      write(STDOUT_FILENO, ">> ", 3);
      itoa(p, i+1);
      memcpy(arg[0], p, sizeof(p));
    }
  
    C[0] = 0;
    while(semaphore_down());
    share(SHARE_WRITE, pnt, C, 20);
    semaphore_up();
  
  }

  int sleep = 0;

  while(true) {
    sleep = curr_timer() + 3;
    while(curr_timer() < sleep);

    while(semaphore_down());
    share(SHARE_READ, 0, C, 20);
    for(int i=0; i<6; i++) {
      p[i] = C[i] + '0';
    }
    write(STDOUT_FILENO, p, n);
  }

  exit( EXIT_SUCCESS );
}
 
