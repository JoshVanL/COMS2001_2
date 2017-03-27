#include "console.h"

/* The following functions are special-case versions of a) writing,
 * and b) reading a string from the UART (the latter case returning 
 * once a carriage return character has been read, or an overall
 * limit reached).
 */
void puts( char* x, int n ) { for( int i = 0; i < n; i++ ) { PL011_putc( UART1, x[ i ], true ); } } 
void gets( char* x, int n ) {
  for( int i = 0; i < n; i++ ) {
    x[ i ] = PL011_getc( UART1, true );
    
    if( x[ i ] == '\x0A' ) {
      x[ i ] = '\x00'; break;
    }
  }
}


/* Since we lack a *real* loader (as a result of lacking a storage 
 * medium to store program images), the following approximates one:
 * given a program name, from the set of programs statically linked
 * into the kernel image, it returns a pointer to the entry point.
 */

extern void main_P3(); 
extern void main_P4(); 
extern void main_P5(); 
extern void main_Factory();
extern void main_Test();
extern void main_Test2();

void* load( char* x ) {
  if     ( 0 == strcmp( x, "P3" ) ) {
    return &main_P3;
  }
  else if( 0 == strcmp( x, "P4" ) ) {
    return &main_P4;
  }
  else if( 0 == strcmp( x, "P5" ) ) {
    return &main_P5;
  }
  else if( 0 == strcmp( x, "Factory" ) ) {
    return &main_Factory;
  }
  else if( 0 == strcmp( x, "Test" ) ) {
    return &main_Test;
  }
  else if( 0 == strcmp( x, "Test2" ) ) {
    return &main_Test2;
  }

  return NULL;
}

/* The behaviour of the console process can be summarised as an
 * (infinite) loop over three main steps, namely
 *
 * 1. write a command prompt then read a command,
 * 2. split the command into space-separated tokens using strtok,
 * 3. execute whatever steps the command dictates.
 */


void main_console() {
  char* p, x[ 1024 ];
  char *arg[5];
  int i;

  while( 1 ) {
    puts( "shell$ ", 8 ); 
    console_writeLCD("shell$ ", 7);
    x[0] = '*';
    //gets( x, 1024 ); 
    while(x[0] == '*') console_readLCD(x);
    p = strtok( x, " " );

    console_command();

    if     ( 0 == strcmp( p, "fork" ) ) {
      void* addr = load( strtok( NULL, " " ) );

      i =0;
      arg[i] = strtok( NULL, " "); 
      while( arg[i] != NULL) {
          i++;
          arg[i] = strtok( NULL, " ");
      }

      if( addr != NULL) {
          pid_t pid = fork();
          // if( 0 == pid ) {
          exec( addr , arg );
          for(int n=0; n<5; n++)arg[n] = NULL;
        //}     
      } else {   
        puts("unknown program\n", 16); 
      }
    } 
    else if( 0 == strcmp( p, "kill" ) ) {
      pid_t pid = atoi( strtok( NULL, " " ) );
      int   s   = atoi( strtok( NULL, " " ) );

      kill( pid, s );
    } 
    else if( 0 == strcmp( p, "killall" ) ) {
      killall();
    } 
    else if( 0 == strcmp( p, "ps" ) ) {
        char* c;
        int ps = processes_count();

        puts ( "pids [", 6);
        itoa(c, ps);
        puts (c, 2);
        puts ( "] : ", 4);
        PL011_putc( UART1, '0', true ); 

        for(int i =1; i<ps; i++) {
          int p = processes_pid(i);
          puts(" ", 1);
          itoa(c, p);
          puts(c, 2);
        }
        puts("\n", 1);
    } 
    else if( 0 == strcmp( p, NULL ) ) {
    }
    else {
      puts( "unknown command\n", 16 );
    }
  }

  exit( EXIT_SUCCESS );
}
