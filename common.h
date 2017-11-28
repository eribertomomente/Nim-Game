/*
 * Percorso del socket per la comunicazione
 */
#define SOCKADDR "/tmp/upperserver.socket"

/*
 * Funzione di utilità per controllare il valore di ritorno di una funzione
 * POSIX e uscire in caso di errore
 */
void check(int result, int exitval, const char *msg);

#define WELCOME "Benvenuto nel NimServer!\n"
#define WAIT "Aspettiamo un tuo avversario!\n"
#define START "Avversario trovato. Giochiamo!\n"
#define YOURTURN "Tocca a te. Fai la tua mossa!\n"
#define CHOOSESTACK "Scegli una pila digitanto '1' o '2': "
#define STACK0CHOSEN "Elementi terminati dalla pila 1. Selezionata pila 0.\n"
#define STACK1CHOSEN "Elementi terminati dalla pila 2. Selezionata pila 1.\n"
#define REMOVEELTS "Selezionare il numero di elementi da rimuovere (1 - %d): "
#define YOUWON "Wow! Hai vinto! :)\n"
#define YOULOST "Ohh! Questa era l'ultima mossa della partita. Hai perso :(\n"
#define ENDGAME "\n*** Partita terminata ***\n"

