#include "Philosopher.h"

#define num 5

//char* C[num];
//int i=0;
//
//void think( uint32_t x ) {
//  write(STDOUT_FILENO, "Thinking\n", 9);
//  for(int c= 0; c < 999999*i; c++) {
//      for(int d=0; d< 99999*i; c++) {
//      }
//  }
//  return;
//}
//
//void eat( uint32_t x) {
//  write(STDOUT_FILENO, "Eating\n", 7);
//  for(int c= 0; c < 999999*i; c++) {
//      for(int d=0; d< 99999*i; c++) {
//      }
//  }
//  return;
//}
//
//void pickup() {
//  while(*C[i] == 1 || *C[(i+1)%num] == 1){
//      read(SHARED_MEM, *C, num);
//      write(STDOUT_FILENO, "Waiting\n", 8);
//  }
//  *C[i] = *C[i+1] = 1;
//  write(SHARED_MEM, *C, num);
//  return;
//}
//
//
//void putdown() {
//  *C[i] = *C[i+1] = 0;
//  write(SHARED_MEM, *C, num);
//  return;
//}

void main_Philosopher(int argc, char* argv[]) {
 //int i = 0;
 //char C[num+1];

 //read(SHARED_MEM, &C, num);
 //write(STDOUT_FILENO, &C, 6);
 //int n =0;
 //while(1) {
 //  if(C[n] == '0'){
 //    i =n;
 //    break;
 //  }
 //  n++;
 //}

 //C[num] = C[i] = '1';
 //char p = i + '0';

 ////write(STDOUT_FILENO, &C, 1);
 //write(SHARED_MEM, &C, 6);

 write(STDOUT_FILENO, argv[0], 5);
 int i = atoi(argv[0]);
 int C[num+1];
 read(SHARED_MEM, &C, num+1);
 C[i] = 1;

 //char* ch;
 //itoa(ch,i);


// long r = (i*1238321);
//
// while(n< 10) {
//   think(r);
//   pickup();
//   eat(r);
//   putdown;
//   n++;
// }


  exit( EXIT_SUCCESS );
}
