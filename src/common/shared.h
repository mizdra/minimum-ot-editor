#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAX_DOCUMENT_SIZE 1024
#define MAX_ACTION_SIZE 1000

int min(int a, int b) { return a > b ? b : a; }

void panic(char *s) {
  perror(s);
  exit(1);
}
