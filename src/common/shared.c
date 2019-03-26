#include "common/shared.h"

#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) { return a > b ? b : a; }

void panic(char *s) {
  perror(s);
  exit(1);
}
