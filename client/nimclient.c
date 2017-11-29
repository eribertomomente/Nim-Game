#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>

#define MAXLENGTH 80

#include "../common.h"
#include "../sock/sock.h"

/*
 *  TODO
 */
int getnatural();

int main() {

  struct match* m;  // partita a nim
  int choice;       // scelta della pila
  char *buffer;

  // apro il socket per la comunicazione
  int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  check(sock, 1, ERR1);

  // imposto l'indirizzo
  struct sockaddr_un addr = {
    .sun_family = AF_LOCAL,
    .sun_path = SOCKADDR
  };

  // mi connetto al server
  int connected = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  check(connected, 2, ERR2);

  // ricezione e stampa a video dei messaggi di benvenuto e inizializzazione del server
  for(int i =0; i <3; i++){
    check(sock_recvmsg(sock), 4, ERR4);
  }
  

  // 1) Ricezione del messaggio del turno
  printf("\n");
  check(sock_recvmsg(sock), 4, ERR4);


  // 2) Ricezione della rappresentazione del match
  check(sock_recvmatch(sock, m), 4, ERR4);
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

      printf(CHOOSESTACK);
      choice = getnatural()-1;
      // mi assicuro di ricevere un input corretto
      while (choice != 0 && choice != 1){
        printf("Errore. %s", CHOOSESTACK);
        choice = getnatural()-1;
      }
      // invio all'arbitro la scelta dell'utente
      buffer = malloc(sizeof(int));
      // converto choice in (char *)
      sprintf(buffer, "%d", choice);
      check(sock_sendmsg(sock, buffer), 4, ERR4);

      free(buffer);

    }
    

    // 4) scelta del numero di elementi da rimuovere

    int elts_tot; // numero di elementi rimasti nella pila prescelta
    elts_tot = match_getelements(m, choice);

    printf(REMOVEELTS, elts_tot);

    // numero di elementi che l'utente vuole rimuovere dalla pila
    int elts_toremove = getnatural(); 
    
    // mi assicuro di ricevere un input corretto
    while (elts_toremove < 1 || elts_toremove > elts_tot){
      printf("Errore. ");
      printf(REMOVEELTS, elts_tot);
      elts_toremove = getnatural();
    }

    
    // invio all'arbitro la scelta dell'utente
    buffer = malloc(sizeof(int));
    // converto choice in (char *)
    sprintf(buffer, "%d", choice);
    check(sock_sendmsg(sock, buffer), 4, ERR4);

    free(buffer);


    // mi preparo all'iterazione i+1-esima

    // 1) Ricezione del messaggio del turno
    printf("\n");
    check( sock_recvmsg(sock), 4, ERR4);


    // 2) Ricezione della rappresentazione del match
    check(sock_recvmatch(sock, m), 4, ERR4);
    match_show(m);

  }

  fprintf(stderr, ENDGAME);


}

int getnatural(){
  int i;
  char buffer[BUFFER_LINE+1];
  fgets(buffer,BUFFER_LINE,stdin);
  i = atoi(buffer);
  return i;
}