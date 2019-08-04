#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "common/network.h"
#include "ot/server.h"
#include "ot/transform.h"

// ファイルディスクリプタ fd に対応するクライアントを切断する
void disconnect(fd_set *fds, fd_set *client_fds, int fd) {
  FD_CLR(fd, fds);
  FD_CLR(fd, client_fds);
  // すでに切断されている場合 (ENOTCONN) は panic しない
  if (shutdown(fd, SHUT_RDWR) == -1 && errno != ENOTCONN)
    PANIC("fail to shutdown client(%d) socket", fd);
  if (close(fd) == -1) PANIC("fail to close client(%d) socket", fd);
}

// アクションをサーバのドキュメントに適用する
int apply_action_to_server(char *document, ACTION action) {
  if (!apply_op(document, action.op)) {
    ERROR("fail to apply operation");
    return 0;
  }
  return 1;
}

// handle_connect, handle_action に渡されるアプリケーションに関する情報
typedef struct {
  SERVER server;
  char *document;
} CONTEXT;

// 新たなクライアントが接続してきた時に呼び出されるハンドラ.
// サーバからクライアントに対し, クライアントのOperation Transformationの
// インスタンスを初期化するための情報を送信する.
bool handle_connect(int client_fd, __attribute__((unused)) fd_set *client_fds,
                    CONTEXT *context) {
  if (write(client_fd, &client_fd, sizeof(int)) == -1) {
    ERROR("fail to send `client_fd`");
    return false;
  }
  if (write(client_fd, &context->server.rev, sizeof(int)) == -1) {
    ERROR("fail to send revision");
    return false;
  }
  if (write(client_fd, context->document, MAX_DOCUMENT_SIZE) == -1) {
    ERROR("fail to send document");
    return false;
  }
  printf("send document = %s\n", context->document);
  return true;
}

// クライアントがアクションを送信してきた時に呼び出されるハンドラ.
// アクションを読み取り, 操作変換してサーバと全クライアントに適用する.
// 適用に失敗した場合はそのアクションを送信してきたクライアントを切断する.
bool handle_action(int client_fd, __attribute__((unused)) fd_set *client_fds,
                   CONTEXT *context) {
  ACTION action;
  if (!recv_action_from_client(&context->server, client_fds, client_fd,
                               &action)) {
    ERROR("fail to recv_action_from_client: client_fd = %d", client_fd);
    return false;
  }

  // デバッグ用に操作変換により得られたアクションを出力する
  printf("xformed action: ");
  print_action(action);
  printf("\n");

  // 操作変換したアクションを適用
  if (!apply_action_to_server(context->document, action))
    PANIC("fail to apply_action_to_server");

  printf("update revision: rev=%d, doc=%s\n", context->server.rev,
         context->document);  // デバッグ用
  return true;
}

void handle_clients(int sockfd, CONTEXT *context) {
  fd_set fds, client_fds;
  FD_ZERO(&fds);
  FD_ZERO(&client_fds);
  FD_SET(sockfd, &fds);

  while (1) {
    fd_set result_fds = fds;
    if (select(FD_SETSIZE, &result_fds, NULL, NULL, NULL) == -1)
      PANIC("fail to `select`");
    usleep(500000);  // 遅延のある環境を再現するために 0.3 秒 sleep する

    for (int fd = 0; fd < FD_SETSIZE; fd++) {
      if (!FD_ISSET(fd, &result_fds)) continue;

      // 新たなクライアントの接続処理
      if (fd == sockfd) {
        printf("#################### sockfd: %d ####################\n",
               sockfd);  // デバッグ用
        // クライアントの接続情報は使わないので, 第2/3引数にはNULLを渡す
        int client_fd = accept(sockfd, NULL, NULL);
        if (client_fd == -1) {
          ERROR("fail to accept");
          continue;
        }
        printf("Accetpted new connection: fd = %d\n", client_fd);  // デバッグ用
        FD_SET(client_fd, &fds);
        FD_SET(client_fd, &client_fds);
        int success = handle_connect(client_fd, &client_fds, context);
        if (!success) disconnect(&fds, &client_fds, client_fd);
        continue;
      }

      // クライアントからアクションを受信したら,
      // サーバのリビジョンに適用できるような形に 操作変換してから実際に適用する
      printf("#################### client_fd: %d ####################\n",
             fd);  // デバッグ用
      int success = handle_action(fd, &client_fds, context);
      if (!success) disconnect(&fds, &client_fds, fd);
    }
  }
}

int main(int argc, char **argv) {
  // listen する
  struct sockaddr_in sin;
  init_sockaddr_in_by_args(argc, argv, &sin);
  int sockfd = bind_and_listen(&sin);

  // context を作成
  SERVER server;
  server_init(&server, 0);
  char document[MAX_DOCUMENT_SIZE] = "Hello!";
  CONTEXT context = {.server = server, .document = document};

  handle_clients(sockfd, &context);

  return 0;
}
