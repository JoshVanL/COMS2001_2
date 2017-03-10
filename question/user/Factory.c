#include "Factory.h" 

extern void main_Philosopher();


void main_Factory(int argc, char* argv[]) {

  void* addr = &main_Philosopher;
  int n = atoi(argv[0]);
  n = 20;
  int C[20];
  char* arg[5];
  arg[0] = "0";
  arg[1] = argv[0];

  for(int i=0; i<20; i++) C[i]=0;


  int *pnt = (int*) share_init();
  itoa(arg[2], *pnt);

  share(SHARE_WRITE, pnt, C, n);
  for(int i=0; i<n; i++) {
    fork();
    exec(addr, arg);
    while(C[9] ==0){
      share(SHARE_READ, pnt, C, n); 
    }
    C[9]=0;
    share(SHARE_WRITE, pnt, C, n);
    write(STDOUT_FILENO, "<<", 2);
    arg[0][0]++;
  }

  C[0] = 1;
  share(SHARE_WRITE, pnt, C, n);

  exit( EXIT_SUCCESS );
}
 
