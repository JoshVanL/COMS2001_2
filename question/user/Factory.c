#include "Factory.h" 
#define num 5

extern void main_Philosopher();


void main_Factory(char* args[]) {
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
  

  fork();
  void* addr = &main_Philosopher;
  exec(addr, "hello");


  exit( EXIT_SUCCESS );
}
 
