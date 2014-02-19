#include <stdio.h>
#include <unistd.h>
#include "libthread1.h"

void EvenOdd()
{
	int i;
	printf( "I'm thread A:\n");
	for ( i = 1; i <= 10; i++ )
	{
		if (i%2==0)
		printf("%d is even\n", i );
		else
		printf("%d is odd\n", i);
		thread1_Yield();
	}
	return;
}

void squares()
{
	int i;
	
	
	printf("I'm thread C: \n");
	for ( i = 0; i < 10; ++ i )
	{
		printf( "%d*%d = %d\n", i, i, i*i );
		thread1_Yield();
	}
}

void fib()
{
	int i,next;
	int first=0;
	int second=1;
	
	printf("I'm thread B: \n");
	printf("fibonacci series:\n ");
	printf( "0\n1\n" );
	for( i = 2; i<=10;  i++ )
	{
		next = first + second;
		printf("%d\n",next);
		first = second;
		second = next;
		thread1_Yield();
	}
}



int main()
{
	
	init_threads();

	spawn_thread1(&EvenOdd);
	//sleep(2);
	spawn_thread1(&fib);
	//sleep(2);	
	spawn_thread1(&squares);	
		
	waitForAllthreads(); 	/*threads are not pre-emptive */  
	
	return 0;
}
