#ifndef __LIBC_H
#define __LIBC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Define a type that that captures a Process IDentifier (PID).

typedef int pid_t;

/* The definitions below capture symbolic constants within these classes:
 *
 * 1. system call identifiers (i.e., the constant used by a system call
 *    to specify which action the kernel should take),
 * 2. signal identifiers (as used by the kill system call), 
 * 3. status codes for exit,
 * 4. standard file descriptors (e.g., for read and write system calls),
 * 5. platform-specific constants, which may need calibration (wrt. the
 *    underlying hardware QEMU is executed on).
 *
 * They don't *precisely* match the standard C library, but are intended
 * to act as a limited model of similar concepts.
 */

#define SYS_YIELD      ( 0x00 )
#define SYS_WRITE      ( 0x01 )
#define SYS_READ       ( 0x02 )
#define SYS_FORK       ( 0x03 )
#define SYS_EXIT       ( 0x04 )
#define SYS_EXEC       ( 0x05 )
#define SYS_KILL       ( 0x06 )
#define SYS_SHARE_INIT ( 0x07 )
#define SYS_SHARE      ( 0x08 )

#define SYS_SEM_DOWN   ( 0x09 )
#define SYS_SEM_UP     ( 0x10 )

#define SYS_TIME       ( 0x11 )
#define SYS_CONS_COM   ( 0x12 )
#define SYS_KILL_ALL   ( 0x13 )

#define SIG_TERM      ( 0x00 )
#define SIG_QUIT      ( 0x01 )

#define EXIT_SUCCESS  ( 0 )
#define EXIT_FAILURE  ( 1 )

#define  STDIN_FILENO ( 0 )
#define STDOUT_FILENO ( 1 )
#define STDERR_FILENO ( 2 )

#define   SHARE_WRITE ( 0 )
#define   SHARE_READ  ( 1 )


// convert ASCII string x into integer r
extern int  atoi( char* x        );
// convert integer x into ASCII string r
extern void itoa( char* r, int x );

// cooperatively yield control of processor, i.e., invoke the scheduler
extern void yield();

// write n bytes from x to   the file descriptor fd; return bytes written
extern int write( int fd, const void* x, size_t n );
// read  n bytes into x from the file descriptor fd; return bytes read
extern int  read( int fd,       void* x, size_t n );

// perform fork, returning 0 iff. child or > 0 iff. parent process
extern int  fork();
// perform exit, i.e., terminate process with status x
extern void exit(       int   x );
// perform exec, i.e., start executing program at address x
extern void exec( const void* x, void* arg );

// signal process identified by pid with signal x
extern int  kill( pid_t pid, int x );

// init share, get shared memory location
extern int share_init(); 

// perform INIT, WRITE or READ from shared mem, return location of that address.
extern int  share( int fd, int pnt, int* x, int n );

// signal wait for semaphore down
extern int semaphore_down();

// signal wait for semaphore up
extern int semaphore_up();

// return timer value
extern int curr_timer();

// Switch program to console for command
extern int console_command();

// Kill all programs
extern int kill_all();

#endif
