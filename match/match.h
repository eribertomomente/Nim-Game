#ifndef MATCH
#define MATCH

/* 
 * This struct represents a Nim match.
 * The stack of each player is represented by the int variables "stack0" and "stack1"
 * "turn" indicates which player is currently playing (0 for player0, 1 for player1).
 */
struct match{
	int stack0, stack1;
	char turn;
};

/* 
 * "match_create" creates and initialize a new Nim match.
 * It generates the stacks of each player randomly (between 5 and 15).
 */
struct match *match_create();

/* 
 * "match_getturn" getter of the field "turn". 
 */
int match_getturn(struct match *m);

/* 
 * "match_isover" returns 1 if the match is over, 0 otherwise.
 */
int match_isover(struct match *m);

/*
 * "match_numberstacks" returns:
 *		1 if stack0 is empty and stack1 not
 *		0 if stack1 is empty and stack0 not
 *		2 if stack0 and stack1 are both not empty
 */
int match_numberstacks(struct match *m);

/*
 * "match_getelements" returns the number of remaining elements from:
 * 		the first stack, if stack = 0
 * 		the second stack, if stack = 1
 */
int match_getelements(struct match *m, int stack);


/* 
 * "match_show" prints on stdout a representation of the match "m".
 */
void match_show(struct match *m);

/* 
 * "match_destroy" deallocates the memory of a match.
 */
void match_destroy(struct match *m);

#endif