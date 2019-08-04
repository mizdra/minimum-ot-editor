#pragma once

#include <arpa/inet.h>
#include <stdlib.h>

#include "common/util.h"

void init_sockaddr_in_by_args(int argc, char** argv, struct sockaddr_in* in) {
  if (argc != 3) PANIC("command line arguments are missing");
  bzero((char*)in, sizeof(*in));
  in->sin_family = PF_INET;
  in->sin_addr.s_addr = inet_addr(argv[1]);
  in->sin_port = htons(atoi(argv[2]));
}

int bind_and_listen(struct sockaddr_in* sin) {
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) PANIC("fail to `socket`");
  if (bind(sockfd, (struct sockaddr*)sin, sizeof(*sin)) == -1)
    PANIC("fail to `bind`");
  if (listen(sockfd, 5) == -1) PANIC("fail to `listen`");
  return sockfd;
}
