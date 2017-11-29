#include "../match/match.h"

/* "sock_sendmsg" sends "msg" through the socket identified by "fd".
 *  It returns 0 if everything works fine, -1 otherwise.
 */
int sock_sendmsg(int fd, char *msg);

/* "sock_recvmsg" receives a message from the socket identified by "fd" and prints it.
 *  It returns 0 if everything works fine, -1 otherwise.
 */
int sock_recvmsg(int fd);

/*
 * "sock_readmsg" receives a message from the socket identified by "fd" and store it in "buffer".
 *  It returns 0 if everything works fine, -1 otherwise.
 */
int sock_readmsg(int fd, char *buffer);

/* "sock_sendmatch" sends the match "m" through the socket identified by "fd".
 *  It returns 0 if everything works fine, -1 otherwise.
 */
int sock_sendmatch(int fd, struct match *m);

/* "sock_recvmatch" receives a match from the socket identified by "fd" and store it in "m".
 *  It returns 0 if everything works fine, -1 otherwise.
 */
int sock_recvmatch(int fd, struct match *m);
