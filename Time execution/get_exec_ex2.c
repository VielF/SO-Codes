/*	
*	Implementa uma tarefa periodica usando clock_nanosleep em modo absoluto
*	Compilar com:	gcc -lrt -o tarefaperiodica1 tarefaperiodica1.c
*	ou
*			gcc -o tarefaperiodica1 tarefaperiodica1.c -lrt
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo


int main(int argc, char* argv[])
{
	struct timespec t;
	int periodo = 500000000; 	// 500ms

	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciarÃ¡ em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

                // Realiza seu trabalho
		printf("Passou um periodo !\n");	

		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
}