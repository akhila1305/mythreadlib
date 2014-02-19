#ifndef LIBTHREAD_H
#define LIBTHREAD_H 1

#define	LF_NOERROR	0
#define	LF_MAXTHREADS	1
#define LF_MALLOCERROR	2
#define LF_CLONEERROR	3
#define	LF_INTHREAD	4
#define LF_SIGNALERROR	5

/* debugging output macro */
#ifdef LF_DEBUG

#include <stdio.h>
#define LF_DEBUG_OUT( string ) fprintf( stderr, "lib_thread debug: " string "\n")
#define LF_DEBUG_OUT1( string, arg ) fprintf( stderr, "lib_thread debug: " string "\n", arg )
#else
#define LF_DEBUG_OUT( string )
#define LF_DEBUG_OUT1( string, arg )
#endif
#endif

#define MAX_THREADS 10			/* The maximum number of threads*/
#define THREAD_STACK (1024*1024)	/* The size of the stack for one thread */


extern void init_threads();				/* initialization to null */
extern int spawn_thread1( void (*func)(void) );		/* Creates a new thread, argument passed is the funtion to be executed by thread */
extern void thread1_Yield();				/* calls sched_yield() */
extern int wwaitForAllthreads();			/* Execute the threads until they all quit. */

