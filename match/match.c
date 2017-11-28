#include <stdlib.h>
#include <stdio.h>

#include "match.h"

struct match *match_create(){
	struct match *m = malloc(sizeof(struct match));
	m->stack0 = (random() % 10) + 5; /* min 5, max 15 */
	m->stack1 = (random() % 10) + 5;
	m->turn = 0;
	return m;
}


int match_getturn(struct match *m){
	return m->turn;
}

int match_isover(struct match *m){
	if (m->stack0==0 && m->stack1==0)
		return 1;
	else
		return 0;
}

int match_numberstacks(struct match *m){
	if (m->stack0==0){
		return 1;
	} else if (m->stack1==0){
		return 0;
	} else{
		return 2;
	}
}

int match_getelements(struct match *m, int stack){
	if (stack==0){
		return m->stack0;
	} else{
		return m->stack1;
	}
}

void match_show(struct match *m){
	printf("---------NIM GAME---------\n");

	//stack0
	printf("Pila 1:\t"); 
	for (int i=0; i<m->stack0; i++) {
		printf("*");
	}
	printf("\n");

	//stack1
	printf("Pila 2:\t"); 
	for (int i=0; i<m->stack1; i++) {
		printf("*");
	}
	printf("\n");
	printf("--------------------------\n");
}

void match_destroy(struct match *m){
	free(m);
}