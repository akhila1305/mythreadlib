#include "libthread1.h"
#include <linux/sched.h> 
#include <signal.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 


/* The thread Structure*/
typedef struct
{
	pid_t pid; /* The pid of the child thread from clone system call */
	void* stack; /* The stack pointer */
} thread1;


static thread1 thread1List[ MAX_THREADS ];	/* The thread "queue" */
static pid_t parentPid;				/* The pid of the parent process */
static int numthreads = 0;			/* The number of active threads */

/* Initialize the threads to null */
void init_threads()
{
	int i;
	for ( i = 0; i < MAX_THREADS;  i++ )
	{
		thread1List[i].pid = 0;
		thread1List[i].stack = 0;
	}
	
	parentPid = getpid();
}


/* sched_yield system call which moves the current process to the end of the process queue. */
void thread1_Yield()
{
	int status;
	status=sched_yield();
	//if (status == 0)
	//	printf("sched yeild sucess");
}


struct thread1Arguments {
	void (*function)();
};

/* Exists to give the proper function type to clone. */
static int thread1Start( void* arg )
{
	struct thread1Arguments* arguments = (struct thread1Arguments*) arg;
	void (*function)() = arguments->function;
	free( arguments );
	arguments = NULL;

	LF_DEBUG_OUT1( "Child created and calling function = %p", arg );
	function();
	return 0;
}

int spawn_thread1( void (*func)(void) )
{
	struct thread1Arguments* arguments = NULL;
	if ( numthreads == MAX_THREADS ) return LF_MAXTHREADS;
	thread1List[numthreads].stack = malloc( THREAD_STACK );		/* Allocate the stack */
	
	if ( thread1List[numthreads].stack == 0 )
	{
		LF_DEBUG_OUT( "Error: Could not allocate stack." );
		return LF_MALLOCERROR;
	}

	
	arguments = (struct thread1Arguments*) malloc( sizeof(*arguments) );	/* Create the arguments structure. */
	
	if ( arguments == 0 ) {
		free( thread1List[numthreads].stack );
		LF_DEBUG_OUT( "Error: Could not allocate thread arguments." );
		return LF_MALLOCERROR;
	}
	arguments->function = func;

	/* Call the clone system call to create the child thread */
	thread1List[numthreads].pid = clone( &thread1Start, (char*) thread1List[numthreads].stack + THREAD_STACK,
		SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, arguments );

	if ( thread1List[numthreads].pid == -1 )
	{
		free( thread1List[numthreads].stack );
		free( arguments );
		LF_DEBUG_OUT( "Error: clone system call failed." );
		return LF_CLONEERROR;
	}
	
	numthreads ++;
	
	return LF_NOERROR;
}

int waitForAllthreads()
{
	pid_t pid;
	int i;
	int threadsRemaining = 0;
		
	/* Check to see if we are in a thread, since we don't get signals in the child threads */
	pid = getpid();
	if ( pid != parentPid ) return LF_INTHREAD;			
		
	/* Wait for the THREADs to quit, then free the stacks */
	while ( numthreads > threadsRemaining )
	{
		pid = wait( 0 );
		if ( pid == -1 )
		{
			LF_DEBUG_OUT( "Error: wait system call failed." );
			exit( 1 );
		}
		
		
		for ( i = 0; i < numthreads; ++ i )	/* Find the thread, free the stack, and swap it with the last one */
		{
			if ( thread1List[i].pid == pid )
			{
				LF_DEBUG_OUT1( "Child thread pid = %d exited", pid );
				numthreads --;
				
				free( thread1List[i].stack );
				if ( i != numthreads )
				{
					thread1List[i] = thread1List[numthreads];
				}
				
				i = -1;
				break;
			}
		}
		if ( i == numthreads )
		{
			LF_DEBUG_OUT1( "Did not find child pid = %d in the thread list", pid ); 
		}
	}
	
	return LF_NOERROR;
}
