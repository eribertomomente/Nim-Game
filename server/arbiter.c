/* 
 * Secondo Progetto per il Laboratorio dell'insegnamento Sistemi Operativi
 * A.A. 2016/2017 - Nicola Rizzo M.128217
 * src/server/arbiter.c
 *
 * Codice della procedura del thread che fa da arbitro.
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>


#include "arbiter.h"
#include "../sock/sock.h"
#include "../match/match.h"
#include "../common.h"

struct match *make_move (int client, struct match *m);

void *arbiter(void *args){

	int cl0_fd = *(int *)args; /* fd dei socket dei client */
    int cl1_fd = *(((int *)args)+1);
	struct match *m = match_create(); /* partita di nim */
	// struct match *previous = match_fakeprev(m); /* stato precedente */

	/* Svolgimento della partita */
	while (!match_isover(m) ){
	// 	// match_cpy(previous,m);
		if (m->turn == 0){

			m = make_move(cl0_fd, m);
			m->turn = 1;		

		} else if (m->turn == 1){

			m = make_move(cl1_fd, m);
			m->turn = 0;
		}
	}

	// annuncio del vincitore
	if (m->turn == 0){
		sock_sendmsg(cl0_fd, YOUWON);
		sock_sendmsg(cl1_fd, YOULOST);
	} else {
		sock_sendmsg(cl1_fd, YOUWON);
		sock_sendmsg(cl0_fd, YOULOST);
	}

	sock_sendmatch(cl0_fd, m);
	sock_sendmatch(cl1_fd, m);
		


	fprintf(stderr, ENDGAME);

	//TODO serve a gestire la terminazione dei client;

	// 1) Invio del messaggio del turno
	sock_sendmsg(cl0_fd,"");
	sock_sendmsg(cl1_fd,"");
	// thread_check(f,cl1_fd,cl0_fd,m,previous);


	// 2) Invio di una partita 
	sock_sendmatch(cl0_fd,m);
	sock_sendmatch(cl1_fd,m);
	// thread_check(f,cl1_fd,cl0_fd,m,previous);

	match_destroy(m);
	close(cl0_fd);
	close(cl1_fd);

	return 0;
}






struct match *make_move (int client, struct match *m){
	int f; /* flag per eventuali errori */


	// 1) Invio del messaggio del turno
	f=sock_sendmsg(client,YOURTURN);
	// thread_check(f,cl1_fd,cl0_fd,m,previous);


	// 2) Invio di una partita 
	f=sock_sendmatch(client,m);
	// thread_check(f,cl1_fd,cl0_fd,m,previous);


	// 3) Scelta della pila
	int stack_choice = 0;

	if (match_numberstacks(m)==1){
		stack_choice = 1;
	} else if (match_numberstacks(m)==2){

		//devo aspettarmi la risposta del messaggio del client sulla scelta della pila
		char *buffer = malloc(sizeof(char));
		f= sock_readmsg(client, buffer);
		check(f, 3, "Errore nella risposta dal client");
		stack_choice = atoi(buffer)-1;
		free(buffer);
  	}

 
  	// 4) scelta del numero di elementi da rimuovere
	char *buffer = malloc(sizeof(int));
	f= sock_readmsg(client, buffer);
	check(f, 3, "Errore nella risposta dal client");
	int elts_choice = atoi(buffer);
	free(buffer);

  	if(stack_choice == 0){
  		m->stack0 -= elts_choice;
  	} else {
  		m->stack1 -= elts_choice;
  	}

  	return m;



}

