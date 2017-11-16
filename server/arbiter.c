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

#include "arbiter.h"
#include "../sock/sock.h"
#include "../match/match.h"
#include "../common.h"


void *arbiter(void *args){

	int flag; /* flag per eventuali errori */
	int cl0_fd = *(int *)args; /* fd dei socket dei client */
    int cl1_fd = *(((int *)args)+1);
	struct match *m = match_create(); /* partita di nim */
	struct match *previous = match_fakeprev(m); /* stato precedente */

	/* Svolgimento della partita */
	while (!match_isover(m) && match_islegit(m,previous)){
		match_cpy(previous,m);
		if (m->turno == 0){

			/* 1) Invio di un messaggio */
			f=sock_sendmsg(cl0_fd,YOURTURN);
			thread_check(f,cl1_fd,cl0_fd,m,previous);

			/* 2) Invio di una partita */
			f=sock_sendmatch(cl0_fd,m);
			thread_check(f,cl1_fd,cl0_fd,m,previous);

			/* 3) Ricezione di una partita */
			f=sock_recvmatch(cl0_fd,m);
			thread_check(f,cl1_fd,cl0_fd,m,previous);

		} else if (m->turno == 1){

			f=sock_sendmsg(cl1_fd,YOURTURN);
			thread_check(f,cl0_fd,cl1_fd,m,previous);

			f=sock_sendmatch(cl1_fd,m);
			thread_check(f,cl0_fd,cl1_fd,m,previous);

			f=sock_recvmatch(cl1_fd,m);
			thread_check(f,cl0_fd,cl1_fd,m,previous);

		}
	}

	/* Gestione della fine della partita */
	if (!match_islegit(m,previous)){ /* Un client è fraudolento */
		if (previous->turno == 0){
			sock_sendmsg(cl0_fd,BLAME); /* 1) */
			m->turno = ERR;
			sock_sendmatch(cl0_fd,m);   /* 2) */
		} else if (previous->turno == 1){
			sock_sendmsg(cl1_fd,BLAME);
			m->turno = ERR;
			sock_sendmatch(cl1_fd,m);
		}
	} else if (match_isover(m)){ /* Terminazione corretta della partita */
		if (m->turno == 0){
			sock_sendmsg(cl0_fd,YOUWON); /* 1) */
			sock_sendmsg(cl1_fd,YOULOST);
			sock_sendmatch(cl0_fd,m);    /* 2) */
			sock_sendmatch(cl1_fd,m);
		} else if (m->turno == 1){
			sock_sendmsg(cl1_fd,YOUWON);
			sock_sendmsg(cl0_fd,YOULOST);
			sock_sendmatch(cl0_fd,m);
			sock_sendmatch(cl1_fd,m);
		}
	}

	close(cl0_fd);
	close(cl1_fd);
	match_destroy(m);
	match_destroy(previous);
	return 0;
}

void thread_check(int ret, int fd1, int fd2, struct match *m, struct match *p){
	if (ret == -1 && fd1 != -1){
		p->turno = ERR;
		sock_sendmsg(fd1,BLAME);
		sock_sendmatch(fd1,p);

		close(fd1);
		close(fd2);
		match_destroy(m);
		match_destroy(p);

		pthread_exit(0);
	}
}
