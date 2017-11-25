/* 
 * Secondo Progetto per il Laboratorio dell'insegnamento Sistemi Operativi
 * A.A. 2016/2017 - Nicola Rizzo M.128217
 * src/server/arbiter.h
 *
 * File di intestazione per la procedura del thread che fa da arbitro.
 */

/* arbiter riceve come argomento un'array di due int, i file descriptor dei
 *   due capi dei socket aperti con i client per la partita di nim che dovrà
 *   portare avanti. */
void *arbiter(void *args);
