/*
 * Percorso del socket per la comunicazione
 */
#define SOCKADDR "/tmp/upperserver.socket"

/*
 * Funzione di utilità per controllare il valore di ritorno di una funzione
 * e uscire in caso di errore
 */
void check(int result, int exitval, const char *msg);

#define WELCOME "Benvenuto nel NimServer!\n"
#define WAIT "Aspettiamo un tuo avversario!\n"
#define START "Avversario trovato. Giochiamo!\n"
#define YOURTURN "Tocca a te. Fai la tua mossa!\n"
#define CHOOSESTACK "Scegli una pila digitanto '1' o '2': "
#define STACK0CHOSEN "Elementi terminati dalla pila 2. Selezionata pila 1.\n"
#define STACK1CHOSEN "Elementi terminati dalla pila 1. Selezionata pila 2.\n"
#define REMOVEELTS "Selezionare il numero di elementi da rimuovere (1 - %d): "
#define YOUWON "Wow! Hai appena fatto l'ultima mossa. Hai vinto! :)\n"
#define YOULOST "Ohh! Il tuo avversario ha appena effettuato l'ultima mossa. Hai perso :(\n"
#define ENDGAME "\n*** Partita terminata ***\n"
#define ERR1 "Errore nell'apertura del socket.\nPartita terminata :(\n"
#define ERR2 "Errore nel bind/connessione di un nuovo client\nPartita terminata :(\n"
#define ERR3 "Errore nell'accettazione di un nuovo client\nPartita terminata :(\n"
#define ERR4 "Errore nello scambio di messaggi\nPartita terminata :(\n"
#define ERR5 "Errore nella chiusura del socket\n"
#define ERRSERVER "Errore nella risposta dal client\n"
#define BUFFER_LINE 3


