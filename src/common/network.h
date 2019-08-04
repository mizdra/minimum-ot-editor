#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#include "common/util.h"

struct sockaddr_in create_sockaddr_in(char *host, int port) {
  struct hostent *hp = gethostbyname(host);

  struct sockaddr_in sin;
  bzero(&sin, sizeof(sin));

  sin.sin_family = PF_INET;
  sin.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
  sin.sin_port = htons(port);

  return sin;
}

int socket_connect(char *host, int port) {
  struct sockaddr_in sin = create_sockaddr_in(host, port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) PANIC("fail to `socket`");
  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    PANIC("fail to `connect`");
  return sockfd;
}

int socket_listen(char *host, int port) {
  struct sockaddr_in sin = create_sockaddr_in(host, port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) PANIC("fail to `socket`");
  if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    PANIC("fail to `bind`");
  if (listen(sockfd, 5) == -1) PANIC("fail to `listen`");
  return sockfd;
}
