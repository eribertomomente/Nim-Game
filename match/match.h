#ifndef MATCH
#define MATCH

/* 
 * This struct represents a Nim match.
 * The stack of each player is represented by the int variables "player0" and "player1"
 * "turn" indicates which player is currently playing (0 for player0, 1 for player1).
 */
struct match{
	int player0, player1;
	char turn;
};

/* 
 * "match_create" creates and initialize a new Nim match.
 * It generates the stacks of each player randomly (between 5 and 15).
 */
struct match *match_create();

/* 
 * "match_copy" copies the instance of struct match "src" in "dest".
 */
void match_copy(struct match *dest, struct match *src);

// TODO: match_fakeprev

/* 
 * "match_play" simulates a step in the game.
 * It removes "elements" elements from the stack of the opposite player 
 *    to the one who is currently playing in the match m.
 */
void match_play(int elements, struct match *m);

/* 
 * "match_getturn" getter of the field "turn". 
 */
int match_getturn(struct match *m);

// TODO: match_islegit

/* 
 * "match_isover" returns 1 if the match is over, 0 otherwise.
 */
int match_isover(struct match *m);

// TODO: match_iserr

/* 
 * "match_show" prints on stdout a representation of the match "m".
 */
void match_show(struct match *m);

/* 
 * "match_destroy" deallocates the memory of a match.
 */
void match_destroy(struct match *m);

#endif