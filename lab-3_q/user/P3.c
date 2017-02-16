#include "P3.h"

void main_P3() {
  int x = 0;
  while( x <100  ) {
    write( STDOUT_FILENO, "P3", 2 ); yield();
    x++;
  }

  exit( EXIT_SUCCESS );
}
