/*		
*	Compilar com:	gcc -lrt -o tarefaperiodica3 tarefaperiodica3.c
*	ou
*			gcc -o tarefaperiodica3 tarefaperiodica3.c -lrt
*
*	Salva o atraso total ateh o *inicio* da execucao
*	Isto inclui overheads, release jitter e possiveis interferencias
*	Salva tambem o tempo de resposta, entre chegada e conclusao	
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define	NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo

#define	N_AMOSTRAS	100		// Numero de amostras (medicoes) coletadas

long atraso_inicio[N_AMOSTRAS];		// Medicoes do atraso ateh inicio em microsegundos
long atraso_fim[N_AMOSTRAS];		// Medicoes do atraso ateh o fim em microsegundos


int main(int argc, char* argv[])
{
	struct timespec t, t_inicio, t_fim;
	int amostra = 0;		// Amostra corrente
	int periodo = 200000000; 	// 200ms

	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciarÃ¡ em 1 segundo
	t.tv_sec++;

	while(amostra < N_AMOSTRAS) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		// Le a hora atual, coloca em t_inicio
		clock_gettime(CLOCK_MONOTONIC ,&t_inicio);

		// Calcula atraso observado em microsegundos
		atraso_inicio[amostra] = 1000000*(t_inicio.tv_sec - t.tv_sec)   +   (t_inicio.tv_nsec - t.tv_nsec)/1000;  	

                // Realiza seu trabalho
		printf("Coletada a amostra %d\n", amostra);	

		// Le a hora atual, coloca em t_fim
		clock_gettime(CLOCK_MONOTONIC ,&t_fim);

		// Calcula o tempo de resposta observado em microsegundos
		atraso_fim[amostra++] = 1000000*(t_fim.tv_sec - t.tv_sec)   +   (t_fim.tv_nsec - t.tv_nsec)/1000;  	

		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}

	printf("Atrasos ateh o inicio da execucao incluem overhead,release jitter, interferencias\n");
	printf("Tempo de resposta vai desde a chegada ateh a conclusao\n\n");
	printf("Atraso inicio           Tempo de resposta\n");

	for( int i=0; i<N_AMOSTRAS; ++i)
		printf("Atraso inicio=%4ldus     Tempo de resposta=%4ldus\n", atraso_inicio[i], atraso_fim[i]);

}