#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "../common.h"
#include "../sock/sock.h"

/*
 *  "getnatural" reads an integer from stdin. 
 *    It returns that int.
 */
int getnatural();

/*
 * "client_check" checks each communication with the server and exit in error cases
 */
void client_check(int result, int exitval, char *msg, int fd);

int main() {

  struct match* m;  // partita a nim
  int choice;       // scelta della pila
  char *buffer;

  // apro il socket per la comunicazione
  int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  check(sock, 1, ERR1);

  signal(SIGPIPE, SIG_IGN);

  // imposto l'indirizzo
  struct sockaddr_un addr = {
    .sun_family = AF_LOCAL,
    .sun_path = SOCKADDR
  };

  // mi connetto al server
  int connected = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  client_check(connected, 2, ERR2, sock);

  // ricezione e stampa a video dei messaggi di benvenuto e inizializzazione del server
  for(int i =0; i <3; i++){
    client_check(sock_recvmsg(sock), 4, ERR4, sock);
  }
  

  // 1) Ricezione del messaggio del turno
  printf("\n");
  client_check(sock_recvmsg(sock), 4, GAMEOVER, sock);

  // 2) Ricezione della rappresentazione del match
  client_check(sock_recvmatch(sock, m), 4, GAMEOVER, sock);
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
    sprintf(buffer, "%d", elts_toremove);
    check(sock_sendmsg(sock, buffer), 4, ERR4);

    free(buffer);


    // mi preparo all'iterazione i+1-esima

    // 1) Ricezione del messaggio del turno
    printf("\n");
    client_check( sock_recvmsg(sock), 4, GAMEOVER, sock);


    // 2) Ricezione della rappresentazione del match
    client_check(sock_recvmatch(sock, m), 4, GAMEOVER, sock);
    match_show(m);

  }

  // chiudo il socket
  check(close(sock), 5, ERR5);

  fprintf(stdin, ENDGAME);


}

int getnatural(){
  int i;
  char buffer[BUFFER_LINE+1];
  fgets(buffer,BUFFER_LINE,stdin);

  // libero lo stdin da eventuali altri caratteri
  fseek(stdin,0,SEEK_END);

  i = atoi(buffer);
  return i;
}

void client_check(int result, int exitval, char *msg, int fd) {
  if(result == -1) {
    close(fd);
    printf("%s\n", msg);
    exit(exitval);
  }
}