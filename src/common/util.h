#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DOCUMENT_SIZE 1024
#define MAX_ACTION_SIZE 1000

int min(int a, int b) { return a > b ? b : a; }

void print_err(const char *format, ...) {
  va_list va;
  va_start(va, format);
  fprintf(stderr, "\033[0;31m");  // set red color
  fprintf(stderr, "error: ");     // print header
  vfprintf(stderr, format, va);   // print error message
  fprintf(stderr, "\033[0m\n");   // reset color
  va_end(va);
}

void panic_with_errno() {
  fprintf(stderr, "\033[0;31m");  // set red color
  perror("error");                // print error message with errno
  fprintf(stderr, "\033[0m\n");   // reset color
  exit(EXIT_FAILURE);
}

void panic(const char *format, ...) {
  va_list va;
  va_start(va, format);
  fprintf(stderr, "\033[0;31m");  // set red color
  fprintf(stderr, "error: ");     // print header
  vfprintf(stderr, format, va);   // print error message
  fprintf(stderr, "\033[0m\n");   // reset color
  va_end(va);
  exit(EXIT_FAILURE);
}
