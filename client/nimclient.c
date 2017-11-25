#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#define MAXLENGTH 80

#include "../common.h"
#include "../sock/sock.h"

int main()
{

  struct match* m;
  int f;
  int choice;

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
   * ricezione e stampa a video dei messaggi di benvenuto e inizializzazione del server
   */
  for(int i =0; i <3; i++){
    f = sock_recvmsg(sock);
    check(f, 3, "connessione al server interrotta");
  }

  

  // 1) Ricezione del messaggio del turno
  printf("\n");
  f = sock_recvmsg(sock);
  check(f, 3, "connessione al server interrotta");


  // 2) Ricezione della rappresentazione del match
  f = sock_recvmatch(sock, m);
  check(f, 3, "connessione al server interrotta");
  match_show(m);

 
  while (!match_isover(m)){

    // 3) scelta della pila
    if (match_numberstacks(m)==0){

      choice=0;
      printf(STACK0CHOSEN);

    } else if(match_numberstacks(m)==1){

      choice=1;
      printf(STACK1CHOSEN);

    } else {

      char buffer;

      printf(CHOOSESTACK);
      scanf(" %c", &buffer);
      // mi assicuro di ricevere un input corretto
      while (buffer != '1' && buffer != '2'){
        printf("Errore. %s", CHOOSESTACK);
        scanf(" %c", &buffer);
      }
      choice = atoi(&buffer)-1;
      sock_sendmsg(sock, &buffer);

    }

    

    // 4) scelta del numero di elementi da rimuovere
    printf(REMOVEELTS);
    int elts_tot = match_getelements(m, choice);
    printf("(1 - %d): ", elts_tot);
    int elts_toremove;
    // implementare il controllo che elts sia un numero e non altro
    scanf(" %d", &elts_toremove);
      // mi assicuro di ricevere un input corretto
    while (elts_toremove < 1 || elts_toremove > elts_tot){
      printf("Errore. %s", REMOVEELTS);
      printf("(1 - %d): ", elts_tot);
      scanf(" %d", &elts_toremove);
    }
    char *buffer = malloc(sizeof(int));
    sprintf(buffer, "%d", elts_toremove);
    sock_sendmsg(sock, buffer);


    // 1) Ricezione del messaggio del turno
    printf("\n");
    f = sock_recvmsg(sock);
    check(f, 3, "connessione al server interrotta");


    // 2) Ricezione della rappresentazione del match
    f = sock_recvmatch(sock, m);
    check(f, 3, "connessione al server interrotta");
    match_show(m);

  }

  fprintf(stderr, ENDGAME);


}

// TODO controllare tutti i sock_*()