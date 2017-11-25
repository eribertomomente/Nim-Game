#include "../match/match.h"

/* sock_sendmsg invia un messaggio qualunque nel socket identificato 
 *   dal file descriptor fornitogli; restituisce 0 se ha mandato tutto il
 *   messaggio correttamente, -1 altrimenti. */
int sock_sendmsg(int fd, char *msg);
/* sock_recv riceve un messaggio qualunque nel socket identificato dal file
 *   descriptor e lo stampa nello standard output; restuisce 0 se è riuscito a
 *   ricevere un messaggio, -1 altrimenti.
 *   NB: se il messaggio è vuoto non stampa nulla nello standard output.
 */
int sock_recvmsg(int fd);

/*
 * TODO
 */
int sock_readmsg(int fd, char *buffer);



/* sock_sendmatch invia la struttura match nel socket fornitogli; restituisce
 *   0 se ha mandato tutta la struttura correttamente, -1 altrimenti.
 */
int sock_sendmatch(int fd, struct match *m);
/* sock_recvmatch riceve una struttura match nel socket identificato dal file
 *   descriptor fornitogli, scrivendola sul puntatore passatogli come argomento;
 *   restituisce 0 se ha successo, -1 altrimenti e non alloca spazio.
 */
int sock_recvmatch(int fd, struct match *m);
