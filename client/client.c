#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLENGTH 80

#include "../common.h"

int main()
{
  // apro il socket per la comunicazione
  int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  check(sock, 1, "socket()");

  // imposto l'indirizzo
  struct sockaddr_un addr = {
    .sun_family = AF_LOCAL,
    .sun_path = SOCKADDR
  };

  // mi connetto al server

  int connected = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  check(connected, 2, "connect()");

  /*
   * ricezione e stampa a video del messaggio di benvenuto del server
   */

  // ricevo la lunghezza del messaggio
  int greetlen = 0;
  int read = recv(sock, &greetlen, sizeof(greetlen), 0);
  check(read, 3, "connessione al server interrotta");

  // ricevo il messaggio
  char *greet = malloc(greetlen);
  read = recv(sock, greet, greetlen, 0);
  check(read, 3, "connessione al server interrotta");

  // stampo il messaggio
  printf("%s", greet);
  free(greet);


  // // acquisizione dei dati da standard input
  // char *line = NULL;
  // while(1) {
  //   line = freadline(stdin, line);
  //   if(!line)
  //     goto close;

  //   /* invio e ricezione della stringa */
  //   int sent = send(sock, line, strlen(line) + 1, 0);
  //   check(sent, 4, "send()");

  //   int read = recv(sock, line, strlen(line) + 1, 0);
  //   check(read, 5, "recv()");

  //   line[read] = 0;

  //   printf("%s\n", line);
//   }

// close:
//    chiusura della connessione 
//   close(sock);
//   return 0;
}
