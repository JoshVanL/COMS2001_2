#include "P3.h"

void main_P3() {
  int x = 0;
  while( x <10000  ) {
    write( STDOUT_FILENO, "P3", 2 );
    x++;
  }

  exit( EXIT_SUCCESS );
}
