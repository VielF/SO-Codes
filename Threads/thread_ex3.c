#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define NUMTHREADS 2
#define VETSIZE 5000

typedef struct {
	int fromidx, length;
}thread_arg, *ptr_thread_arg;

pthread_t threads[NUMTHREADS];
thread_arg arguments[NUMTHREADS];
int nums[VETSIZE];
int sum;

void *thread_func(void *arg);

int main(int argc, char **argv){
	int i, length, remainder;
	sum = 0; length = VETSIZE / NUMTHREADS;
	remainder = VETSIZE % NUMTHREADS;
	for(i=0; i<NUMTHREADS; i++) {
		arguments[i].fromidx = i * length;
		arguments[i].length = length;
		if(i == (NUMTHREADS – 1)) arguments[i].length += remainder;
		pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i]));
	}
	for(i=0; i<NUMTHREADS; i++) pthread_join(threads[i], NULL);
	printf(“A soma dos numeros do vetor eh %d\n”, sum);
}

void *thread_func(void *arg) {
	ptr_thread_arg argument = (ptr_thread_arg)arg;
	int i, localsum = 0, endidx;
	
	endidx = argument->fromidx + argument->length;
	
	for(i=argument->fromidx; i<endidx; i++){
		localsum += nums[i];
	}
	sum += localsum;
}