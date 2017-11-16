#include <stdio.h>      /* I/O */
#include <string.h>     /* strcpy */
#include <sys/types.h>  /* per i socket unix */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>     /* unlink */
#include <stdlib.h>     /* exit e srandom */
#include <pthread.h>
#include <signal.h>     /* segnali */

#include "../common.h"
#include "../sock/sock.h"
#include "arbiter.h"
#include "thrarray.h"

#define SETUP "Server in ascolto.\n"
// #define CONNECTED "Opponent just found."

// /* Variabile file-scoped per gli handler */
// struct thrarray *pt;

// /* Signal handler */
// void quit(int sig){
// 	for (int i=0; i<pt->size; i++){
// 		pthread_join(thrarray_get(pt)[i],NULL);
// 	}
// 	thrarray_destroy(pt);
// 	unlink(SOCKET_PATH);
// 	unlink(SOCKET_ALT_PATH);
// 	printf("\nServer terminato.\n");
// 	exit(0);
// }


// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################

#include "../common.h"

void handle_sigchld(int);

int main (int argc, char **argv)
{
  // imposto l'handler per SIGCHLD, in modo da non creare processi zombie
  signal(SIGCHLD, handle_sigchld);

  // apro il socket di ascolto
  int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  check(sock, 1, "socket()");

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
  check(bind(sock, (struct sockaddr *)&addr, sizeof addr), 2, "bind()");

  // Abilito effettivamente l'ascolto, con un massimo di 5 client in attesa
  listen(sock, 5);

  fprintf(stderr, SETUP);

  // continuo all'infinito ad aspettare client
  while (1)
  {
    struct sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);
    int fd = accept(sock, (struct sockaddr *)&client_addr, &client_len);
    check(fd, 3, "accept()");

    /*
     * ogni volta che il server accetta una nuova connessione,
     * quest'ultima viene gestita da un nuovo processo figlio
     */
    pid_t pid = fork();
    check(pid, 4, "fork()");

    // il figlio gestisce la connessione, il padre torna subito in ascolto
    if(!pid) {
      fprintf(stderr, "Aperta connessione (PID %d).\n",getpid());

      char greet[] = "Benvenuto al gioco del Nim!\n";
      int greetlen = strlen(greet) + 1;
      send(fd, &greetlen, sizeof(greetlen), 0); // Invio lunghezza del messaggio
      send(fd, greet, greetlen, 0); // Invio il messaggio

      close(fd); // Alla fine chiudiamo la connessione

      fprintf(stderr, "Chiusa connessione (PID %d).\n",getpid());
    }
  }
}

#define BLOCKSIZE 40

void handle_sigchld(int x) {
  int saved_errno = errno;
  while (waitpid(-1, 0, WNOHANG) > 0) {}
  errno = saved_errno;
}


// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################
// ###########################################################


// int main()
// {
// 	printf("Welcome to the Nim Game. Let's play!");

	
// 	// sigset_t sigset, oldset; /* maschere dei segnali */
// 	// struct sigaction s;  /* sigaction */
// 	int flag;  // valore di ritorno per eventuali errori 
// 	int sock; /* futuro fd del socket */
// 	struct sockaddr_un addr; /* indirizzo */
	

// 	/* Impostazione seme di random */
// 	// TODO check this
// 	srandom(123456);

// 	/* SOCKET - Inizializzazione indirizzo del socket */
	
// 	#ifdef __APPLE__ /* Compatibilità MacOS */
// 	 addr.sun_len = strlen(SOCKET_PATH)+1;
// 	#endif
// 	 addr.sun_family = AF_LOCAL;
// 	 strcpy(addr.sun_path,SOCKET_PATH);
	

// 	/* SOCKET - Inizializzazione del fd del socket in ascolto */

// 	sock = socket(AF_LOCAL, SOCK_STREAM, 0);

// 	// TODO check this out
// 	check(sock,"socket()");


// 	/* SIGNAL - Impostazione della sigaction */

// 	// s.sa_handler = quit;
// 	// sigemptyset(&s.sa_mask);
// 	// s.sa_flags = 0;
// 	// sigaction(SIGINT,&s,NULL);


// 	/* SIGNAL - Blocco di SIGINT, salvataggio della vecchia maschera */

// 	// sigemptyset(&sigset);
// 	// sigaddset(&sigset, SIGINT);
// 	// pthread_sigmask(SIG_BLOCK, &sigset, &oldset);

// 	/* SIGNAL - Ripristino della maschera */
// 	//pthread_sigmask(SIG_SETMASK, &oldset, NULL);

// 	/* SIGNAL - Inizializzazione dell'array di thread_t */
// 	pt = thrarray_create();


// 	/* SOCKET - In caso dei socket siano rimasti aperti */

// 	//TODO check this out
// 	// unlink(SOCKET_PATH);
// 	// unlink(SOCKET_ALT_PATH);

// 	 SOCKET - Collegamento al socket 
// 	flag = bind(sock,(const struct sockaddr *)&addr,sizeof(addr));
// 	if (flag){
// 		// #ifdef __APPLE__ /* Compatibilità MacOS */
// 		// addr.sun_len = strlen(SOCKET_ALT_PATH)+1;
// 		// #endif
// 		strcpy(addr.sun_path,SOCKET_ALT_PATH);
// 		flag = bind(sock,(const struct sockaddr *)&addr,sizeof(addr));
// 	}
// 	check(flag,"bind()");

// 	/* SOCKET - Ascolto sul socket */
// 	flag = listen(sock,5);
// 	check(f,"listen()");

// 	while (1){
// 		struct sockaddr_un cl0_addr, cl1_addr; /* indirizzi */
// 		int cl0_fd, cl1_fd;                    /* file descriptor */
// 		socklen_t cl0_len=sizeof(cl0_addr);    /* lunghezza indirizzi */
// 		socklen_t cl1_len=sizeof(cl1_addr);
// 		pthread_t thr;                         /* thread arbitro */

// 		/* SOCKET - Ricezione di due client, invio di due messaggi */
// 		cl0_fd = accept(sock,(struct sockaddr *)&cl0_addr,&cl0_len);
// 		sock_sendmsg(cl0_fd,WAITING);
// 		cl1_fd = accept(sock,(struct sockaddr *)&cl1_addr,&cl1_len);
// 		sock_sendmsg(cl1_fd,"");
// 		sock_sendmsg(cl0_fd, CONNECTED);
// 		sock_sendmsg(cl1_fd, CONNECTED);

// 		/* Argomenti per l'arbitro */
// 		int pair[2] = {cl0_fd,cl1_fd};

// 		/* SIGNAL - Blocco di SIGINT */
// 		pthread_sigmask(SIG_BLOCK, &sigset, NULL);

// 		/* Thread arbitro */
// 		pthread_create(&thr,NULL,arbiter,(void *)pair);
// 		thrarray_add(thr,pt);

// 		/* SIGNAL - Ripristino della maschera */
// 		pthread_sigmask(SIG_SETMASK, &oldset, NULL);
// 	}
// }
