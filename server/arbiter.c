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


void *arbiter(void *args){

	// fd dei socket dei client
	int client0_fd = *(int *)args;
    int client1_fd = *(((int *)args)+1);

    // la partita di nim
	struct match *m = match_create();

	// svolgimento della partita
	while (!match_isover(m) ){

		if (m->turn == 0){
			m = make_move(client0_fd, m);
			m->turn = 1;		

		} else if (m->turn == 1){
			m = make_move(client1_fd, m);
			m->turn = 0;
		}
	}

	// annuncio del vincitore:
	// il client che ha effettuato l'ultima mossa ha "concesso" il turno all'avversario
	// -> il client indicato da turn, a questo punto, ha vinto la partita
	if (m->turn == 0){
		sock_sendmsg(client0_fd, YOUWON);
		sock_sendmsg(client1_fd, YOULOST);
	} else {
		sock_sendmsg(client1_fd, YOUWON);
		sock_sendmsg(client0_fd, YOULOST);
	}

	// invio la partita ai 2 client per mostrare che la partita è realmente finita
	sock_sendmatch(client0_fd, m);
	sock_sendmatch(client1_fd, m);
		
	fprintf(stderr, ENDGAME);

	match_destroy(m);
	close(client0_fd);
	close(client1_fd);

	return 0;
}




struct match *make_move (int client, struct match *m){
	
	// flag per eventuali errori
	int f;

	// 1) Invio del messaggio del turno
	f=sock_sendmsg(client,YOURTURN);

	// 2) Invio di una partita 
	f=sock_sendmatch(client,m);


	// 3) Scelta della pila
	int stack_choice = 0;
	if (match_numberstacks(m)==1){

		stack_choice = 1;
	
	} else if (match_numberstacks(m)==2){ // se sono rimaste entrambe le pile
		//devo aspettarmi la risposta del messaggio del client sulla scelta della pila

		char *buffer = malloc(sizeof(char));

		f = sock_readmsg(client, buffer);
		check(f, 3, "Errore nella risposta dal client");

		// cast della risposta dal client in int
		stack_choice = atoi(buffer)-1;

		free(buffer);

  	}

 
  	// 4) scelta del numero di elementi da rimuovere
	char *buffer = malloc(sizeof(int));

	f= sock_readmsg(client, buffer);
	check(f, 3, "Errore nella risposta dal client");
	
	// cast della risposta dal client in int
	int elts_toremove = atoi(buffer);
	
	free(buffer);


	// eliminazione degli elementi dalla pila
  	if(stack_choice == 0){
  		m->stack0 -= elts_toremove;
  	} else {
  		m->stack1 -= elts_toremove;
  	}

  	return m;

}

