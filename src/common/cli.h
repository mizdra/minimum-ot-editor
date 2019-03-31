#pragma once

#include <arpa/inet.h>
#include <stdlib.h>

#include "common/util.h"

void parse_args(int argc, char** argv, in_addr_t* addr, in_port_t* port) {
  if (argc != 3) panic("command line arguments are missing.");
  *addr = inet_addr(argv[1]);
  *port = htons(atoi(argv[2]));
}
