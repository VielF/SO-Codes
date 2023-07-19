/*	
*	Ilustra a criacao de threads e uso de mutex
*	Compilar com:	gcc -lpthread -o pthreads-tela pthreads-tela.c
*	ou
*			gcc -o pthreads-tela pthreads-tela.c -lpthread
*
*/

#include	<pthread.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>

/*************************************************************************
*	Monitor responsavel pelo acesso ao valor total
*
*	Uma unica funcao, usada para somar 'delta' no valor total
***/

int valor_total = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

int alteraValor( int delta) {
	int novo_valor;

	pthread_mutex_lock(&em);
	valor_total += delta;
	novo_valor = valor_total;
	pthread_mutex_unlock(&em);
	return novo_valor;
}

void defineValor( int novo) {
	pthread_mutex_lock(&em);
	valor_total = novo;
	pthread_mutex_unlock(&em);
}
/*************************************************************************/

/*************************************************************************
*	Monitor responsavel pelo acesso a tela
*
***/

pthread_mutex_t tela = PTHREAD_MUTEX_INITIALIZER;

void aloca_tela( void) {
	pthread_mutex_lock(&tela);
	}

void libera_tela( void) {
	pthread_mutex_unlock(&tela);
	}

/*************************************************************************/

/***
*	Thread que mostra status na tela
***/
void thread_mostra_status(void) {
	int ultimo;
	while(1){
		sleep(1);
		aloca_tela();
		ultimo = alteraValor(+1);
		printf("Ultimo lido foi %d, digite <enter> para alterar\n", ultimo);
		libera_tela();	
	}
}

/***
*	Thread que le teclado
***/
void thread_le_teclado(void) {
	int novo;
	char teclado[1000];
	while(1){
		fgets( teclado, 1000, stdin);
		aloca_tela();
		printf("Digite novo valor:\n");
		fgets( teclado, 1000, stdin);
		defineValor(atoi(teclado));
		libera_tela();
	}
}

int main( int argc, char *argv[]) {

        pthread_t t1, t2;

        pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
        pthread_create(&t2, NULL, (void *) thread_le_teclado, NULL);

	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
}