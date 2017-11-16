/* arbiter riceve come argomento un'array di due int, i file descriptor dei
 *   due capi dei socket aperti con i client per la partita di nim che dovrà
 *   portare avanti. */
void *arbiter(void *args);

/* thread_check controlla il valore ret e gestisce gli errori nella
 *   comunicazione con i client; avverte il client che fino a quel momento si
 *   è comportato normalmente dell'errore e libera lo spazio allocato. */
void thread_check(int ret, int fd1, int fd2, struct match *m, struct match *p);
