#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void check(int result, int exitval, const char *msg) {
  if(result == -1) {
    perror(msg);
    exit(exitval);
  }
}