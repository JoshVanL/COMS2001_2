#include "Philosopher.h"

void main_Philosopher(int argc, char* argv[]) {
 char* a = argv[0];
 char* b = argv[1];
 char* c = argv[2];
 write(STDOUT_FILENO, a, sizeof(a));
 write(STDOUT_FILENO, b, sizeof(b));
 int i = atoi(a);
 int n = atoi(b);
 int* pnt = (int*) atoi(c);

 int C[n];




 share(SHARE_READ, pnt, C, n); 

 C[i] =1;
 share(SHARE_WRITE, pnt, C, n); 



  exit( EXIT_SUCCESS );
}
