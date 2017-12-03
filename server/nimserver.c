#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h> // per struct sockaddr_un
#include <sys/socket.h>
#include <pthread.h>

#include "../common.h"
#include "../sock/sock.h"
#include "arbiter.h"

void handle_sigchld(int);

int main (int argc, char **argv)
{
  // imposto l'handler per SIGCHLD, in modo da non creare processi zombie
  signal(SIGCHLD, handle_sigchld);

  // imposto seme random
  srandom(25122017);

  // apro il socket di ascolto
  int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  check(sock, 1, ERR1);

  // imposto l'indirizzo del socket
  struct sockaddr_un addr = {
    .sun_family = AF_LOCAL,
    .sun_path = SOCKADDR
  };

  // mi preoccupo di rimuovere il file del socket in caso esista già.
  // se è impegnato da un altro server non si potrà rimuovere, ma bind()
  // successivamente mi darà errore
  unlink(SOCKADDR);

  // lego l'indirizzo al socket di ascolto
  check(bind(sock, (struct sockaddr *)&addr, sizeof addr), 2, ERR2);

  // Abilito effettivamente l'ascolto, con un massimo di 5 client in attesa
  listen(sock, 5);

  fprintf(stderr, "In ascolto.\n");


  // continuo all'infinito ad aspettare client
  while (1)
  {

    // i due client che si dovranno connettere
    struct sockaddr_un client0_addr;
    struct sockaddr_un client1_addr;

    // file descriptor per i due client
    int client0_fd;
    int client1_fd;

    // lunghezza indirizzi
    socklen_t client0_len=sizeof(client0_addr); 
    socklen_t client1_len=sizeof(client1_addr);


    /* SOCKET - Ricezione di due client, invio di due messaggi */

    client0_fd = accept(sock, (struct sockaddr *)&client0_addr, &client0_len);
    check(client0_fd, 3, ERR3);
    fprintf(stderr, "Client_0 accettato.\n");

    check(sock_sendmsg(client0_fd,WELCOME), 4, ERR4);
    check(sock_sendmsg(client0_fd,WAIT), 4, ERR4);

    client1_fd = accept(sock,(struct sockaddr *)&client1_addr,&client1_len);
    check(client1_fd, 3, ERR3);
    fprintf(stderr, "Client_1 accettato.\n");

    check(sock_sendmsg(client1_fd,WELCOME), 4, ERR4);
    check(sock_sendmsg(client1_fd,""), 4, ERR4);
    check(sock_sendmsg(client1_fd,START), 4, ERR4);

    check(sock_sendmsg(client0_fd,START), 4, ERR4);


    //thread arbitro
    pthread_t thr;    

    // Argomenti per l'arbitro
    int pair[2] = {client0_fd,client1_fd};

    // Thread arbitro
    pthread_create(&thr,NULL,arbiter,(void *)pair);

    // aspetto la sua terminazione
    pthread_join(thr, NULL);

    return 0;
  }
}


void handle_sigchld(int x) {
  int saved_errno = errno;
  while (waitpid(-1, 0, WNOHANG) > 0) {}
  errno = saved_errno;
}
