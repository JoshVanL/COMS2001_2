#include "P5.h"

void main_P5() {
  while( 1 ) {
    write( STDOUT_FILENO, "P5", 2 ); yield();
  }

  exit( EXIT_SUCCESS );
}
