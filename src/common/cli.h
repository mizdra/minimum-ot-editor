#pragma once

#include <arpa/inet.h>
#include <stdlib.h>

#include "common/util.h"

void parse_args(int argc, char **argv, char *host, int *port) {
  if (argc != 3) PANIC("command line arguments are missing");
  strcpy(host, argv[1]);
  *port = atoi(argv[2]);
}
