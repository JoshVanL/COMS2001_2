#include "P4.h"

void main_P4() {
  while( 1 ) {
    write( STDOUT_FILENO, "P4", 2 ); yield();
  }

  exit( EXIT_SUCCESS );
}
