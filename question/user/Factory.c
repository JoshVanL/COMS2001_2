#include "Factory.h" 

extern void main_Philosopher();


void main_Factory(int argc, char* argv[]) {

  void* addr = &main_Philosopher;
  int n = atoi(argv[0]);
  int C[n];
  char* arg[5];
  arg[0] = "0";
  arg[1] = argv[0];


  for(int i=0; i<n; i++) C[i] = 0;

  write(SHARED_MEM, C, n);
  read(SHARED_MEM, C, n);
   for(int i=0; i<n; i++) {
     fork();
     exec(addr, arg);
     while(C[i] ==0){
         read(SHARED_MEM, C, n);
     }
     write(STDOUT_FILENO, "<<", 2);
     arg[0][0]++;
   }
  

  exit( EXIT_SUCCESS );
}
 
