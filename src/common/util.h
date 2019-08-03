#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MAX_DOCUMENT_SIZE 1024
#define MAX_ACTION_SIZE 1000

#define PANIC(fmt, ...)                                                     \
  {                                                                         \
    err_msg(__FILE__, __FUNCTION__, __LINE__, "panic", fmt, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);                                                     \
  }
#define ERROR(fmt, ...) \
  err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)
#define WARNING(fmt, ...) \
  err_msg(__FILE__, __FUNCTION__, __LINE__, "warning", fmt, ##__VA_ARGS__)

#define RED "\033[0;31m"
#define BOLD "\033[1m"
#define RESET_COLOR "\033[0m"

int min(int a, int b) { return a > b ? b : a; }

void err_msg(const char *file, const char *function, int line, const char *type,
             const char *fmt, ...) {
  // print error message
  fprintf(stderr, RED);
  fprintf(stderr, "%s: ", type);
  va_list va;
  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  va_end(va);
  fprintf(stderr, RESET_COLOR);
  fputc('\n', stderr);

  // print error source
  fprintf(stderr, BOLD);
  fprintf(stderr, " --> `%s` (%s:%d)", function, file, line);
  fprintf(stderr, RESET_COLOR);
  fputc('\n', stderr);

  // print errno
  if (errno != 0) {
    fprintf(stderr, " = errno: %s(%d)", strerror(errno),
            errno);       // print errno
    fputc('\n', stderr);  // new line
    errno = 0;
  }
}
