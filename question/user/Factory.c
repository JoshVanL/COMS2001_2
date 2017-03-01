#include "Factory.h" 
#define num 5

extern void main_Philosopher();


void main_Factory(int argc, char* argv[]) {
// char C[num+1];
// char P[6];
// char* arg[10];
// void* addr = &main_Philosopher;
//
// for (int i = 0; i < num+1; i++) {
//     C[i] = '0';
//     P[i] = '1';
// }
//
//for(int i= 0; i< num; i++) {
//  C[i] = '1';
//  write(SHARED_MEM, &C, num+1);
//  fork();
//  exec(addr);
//  while(C[num] == '0'){
//    read(SHARED_MEM, C, num+1);
//  }
//  write(STDOUT_FILENO, &C, 6);
//  C[num] = '0';
//}
//
//write(STDOUT_FILENO, "DONE&", 5);
// 

  void* addr = &main_Philosopher;
  char* arg[1];

  int C[num+1];
  for(int i=0; i<=num; i++) C[i] = 0;
  write(SHARED_MEM, &C, num+1);


  for(int i=0; i<1; i++) {
    itoa(arg[0], i);
    arg[0] = "hello";
    //write(STDOUT_FILENO, arg[0], 1);
    //write(STDOUT_FILENO, "\n", 1);
    fork();
    exec(addr, arg);
    while(C[i] ==0){
        read(SHARED_MEM
  }



  exit( EXIT_SUCCESS );
}
 
