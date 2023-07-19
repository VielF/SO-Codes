/**
 * Example of openmp parallel region
 * 
 * To compile, enter:
 *
 *	gcc -fopenmp openmp.c
 *
 * You should see the message "I am a parallel region" for each
 * processing core on your system.
 *
 * For those using a virtual machine, make sure you set the number of
 * processing cores > 1 to see parallel execution of the parallel region.
 */

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   int thread_id;
	/* sequential code */

	#pragma omp parallel
	{
      thread_id = omp_get_thread_num();
		printf("I am a parallel region of Thread: %d\n", thread_id);
	}

	/* sequential code */

	return 0;
}