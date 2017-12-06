#ifndef ARBITER
#define ARBITER
/* 
 * "arbiter" gets as arguments two file desc of the client's sockets playing a nim match
 *  It conduces a nim game giving each player its own turn until the match is over.
 */
void *arbiter(void *args);

/*
 * "make_move" simulates a step on the nim game.
 * "m" is the current match
 * "client" is the fd of the player who is currently playing
 */
struct match *make_move (int main_client, int other_client, struct match *m);

#endif