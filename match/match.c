#include <stdlib.h>
#include <stdio.h>

#include "match.h"

struct match *match_create(){
	struct match *m = malloc(sizeof(struct match));
	m->player0 = (random() % 10) + 5; /* min 5, max 15 */
	m->player1 = (random() % 10) + 5;
	m->turn = 0;
	return m;
}

void match_copy(struct match *dest, struct match *src){
	dest->player0 = src->player0;
	dest->player1 = src->player1;
	dest->turn = src->turn;
}

//TODO: match_fakeprev

void match_play(int elements, struct match *m){
	if (m->turn == 0){
		m->player1 -= elements;
		m->turn = 1;
	} else {
		m->player0 -= elements;
		m->turn = 1;
	}
}

int match_getturn(struct match *m){
	return m->turn;
}

// TODO: match_islegit

int match_isover(struct match *m){
	if (m->player0==0 && m->player1==0)
		return 1;
	else
		return 0;
}

// TODO: match_iserr

void match_show(struct match *m){
	printf("NIM GAME");

	//player0
	printf("\tPlayer 1:\t"); 
	for (int i=0; i<m->player0; i++) {
		printf("*");
	}
	printf("\n");

	//player1
	printf("\tPlayer 2:\t"); 
	for (int i=0; i<m->player1; i++) {
		printf("*");
	}
	printf("\n");
}

void match_destroy(struct match *m){
	free(m);
}