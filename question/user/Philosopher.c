#include "Philosopher.h"

void main_Philosopher(int argc, char* argv[]) {
 char* a = argv[0];
 char* b = argv[1];
 write(STDOUT_FILENO, a, sizeof(a));
 write(STDOUT_FILENO, b, sizeof(b));
 int i = atoi(a);
 int n = atoi(b);

 int C[n];




 read(SHARED_MEM, C, n);

 C[i] =1;
 write(SHARED_MEM, C, n);



  exit( EXIT_SUCCESS );
}
