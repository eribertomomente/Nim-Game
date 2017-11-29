#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "../common.h"
#include "../match/match.h"

/* L'opzione SO_NOSIGPIPE su ogni send sopprime il segnale SIGPIPE che verrebbe
 *   generato se l'altro capo della connessione fosse chiuso.
 * L'opzione MSG_WAITALL su ogni recv impone il blocco della operazione
 *   fino alla ricezione dell'intero messaggio. */
int sock_sendmsg(int fd, char *msg){
	/* invio lunghezza messaggio*/
	int len = strlen(msg)+1;
	ssize_t s = send(fd,&len,sizeof(len),SO_NOSIGPIPE);
	if (s!=sizeof(len)){
		return -1;
	}
	/* invio messaggio */
	s = send(fd,msg,len,SO_NOSIGPIPE);
	if (s!=len){
		return -1;
	}
	return 0;
}

int sock_recvmsg(int fd){
	/* ricezione lunghezza messaggio */
	int len = 0;
	ssize_t s = recv(fd,&len,sizeof(len),MSG_WAITALL);
	if (s!=sizeof(len)){
		return -1;
	}

	/* ricezione messaggio */
	char* msg = malloc(len*sizeof(char));
	s = recv(fd,msg,len,MSG_WAITALL);
	if (s!=len){
		return -1;
	}

	fprintf(stderr, "%s", msg);
	free(msg);

	return 0;
}

int sock_readmsg(int fd, char *buffer){

	/* ricezione lunghezza messaggio */
	int len = 0;
	ssize_t s = recv(fd,&len,sizeof(len),MSG_WAITALL);
	if (s!=sizeof(len)){
		return -1;
	}
	/* ricezione messaggio */
	s = recv(fd,buffer,len,MSG_WAITALL);
	if (s!=len){
		return -1;
	}
	return 0;
}

int sock_sendmatch(int fd, struct match *m){
	/* invio lunghezza struct match */
	int len = sizeof(struct match);
	ssize_t s = send(fd,&len,sizeof(len),SO_NOSIGPIPE);
	if (s!=sizeof(len)) {
		return -1;
	}
	/* invio struct match */
	s = send(fd,m,len,SO_NOSIGPIPE);
	if (s!=sizeof(struct match)){
		return -1;
	}
	return 0;
}

int sock_recvmatch(int fd,struct match *m){
	/* ricezione lunghezza struct match */
	int len = 0;
	ssize_t s = recv(fd,&len,sizeof(len),MSG_WAITALL);
	if (s!=sizeof(len) || len!=sizeof(struct match)){
		return -1;
	}
	/* ricezione struct match */
	s = recv(fd,m,len,MSG_WAITALL);
	if (s!=sizeof(struct match)){
		return -1;
	}
	return 0;
}
