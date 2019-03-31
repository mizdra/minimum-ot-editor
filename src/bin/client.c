#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "common/cli.h"
#include "editor/init.h"
#include "editor/input.h"
#include "editor/render.h"
#include "ot/client.h"

// 標準入力からの入力をハンドリングする
int handle_stdin(int fd, CLIENT *client, EDITOR *editor) {
  // オペレーションを読み取る
  OP op = scan_operation(editor);

  // カーソル移動や無効な操作を受け取った場合は,
  // サーバに送信せずにselect文まで戻る
  if (is_nop(op)) {
    render_screen(editor);
    return 1;
  }

  // 操作に対応するアクションを生成し, サーバに送信する
  ACTION action = action_init(editor->my_client_id, client->rev, op);
  if (!send_action_to_server(client, fd, action)) {
    fprintf(stderr, "fail to send action: ");
    print_action(action);
    fprintf(stderr, "\n");
    return 0;
  }

  // 送信が完了したらローカルのドキュメントにアクションを適用する
  if (!apply_action_to_client(editor, action)) return 0;

  return 1;
}

// サーバからの入力をハンドリングする
int handle_server(int fd, CLIENT *client, EDITOR *editor, int my_client_id) {
  // サーバからアクションを受信したら,
  // クライアントのリビジョンに適用できるような形に
  // 操作変換してから実際に適用する

  // アクションをサーバから受信し, 操作変換したものを action に格納する
  ACTION action;
  if (!recv_action_from_server(client, fd, my_client_id, &action)) {
    fprintf(stderr, "fail to receive action: ");
    print_action(action);
    fprintf(stderr, "\n");
    return 0;
  }

  // デバッグ用に操作変換により得られたアクションを出力する
  printf("xformed action: ");
  print_action(action);
  printf("\n");

  // 操作変換したアクションを適用
  if (!apply_action_to_client(editor, action)) {
    fprintf(stderr, "fail to apply action: ");
    print_action(action);
    fprintf(stderr, "\n");
    return 0;
  }

  return 1;
}

// サーバに接続した直後に呼び出す関数.
// サーバからクライアントのOperation Transformationのインスタンスを初期化
// するための情報を受信する.
int on_connect(int fd, int *my_client_id, int *rev, char *document) {
  // サーバから割り振られたクライアントごとのidを受け取る
  if (read(fd, my_client_id, sizeof(int)) <= 0) {
    fprintf(stderr, "fail to receive my_client_id from server\n");
    return 0;
  }

  // サーバからドキュメントのリビジョンを受け取る
  if (read(fd, rev, sizeof(int)) <= 0) {
    fprintf(stderr, "fail to receive revision from server\n");
    return 0;
  }

  // サーバからドキュメントを受け取る
  if (read(fd, document, MAX_DOCUMENT_SIZE) <= 0) {
    fprintf(stderr, "fail to receive document from server\n");
    return 0;
  }

  // デバッグ用のログを出力
  printf("recv document = %s\n", document);
  return 1;
}

int service(int fd, EDITOR *editor) {
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  FD_SET(fd, &fds);

  int rev;
  if (!on_connect(fd, &editor->my_client_id, &rev, editor->document)) return 0;

  render_screen(editor);

  CLIENT client;
  client_init(&client, rev);

  while (1) {
    fd_set result_fds = fds;  // select の結果を書き込むための fd_set を用意する
    if (select(FD_SETSIZE, &result_fds, NULL, NULL, NULL) < 0) {
      fprintf(stderr, "fail to select\n");
      return 0;
    }

    // 標準入力から入力があったら handle_stdin を,
    // サーバから入力があったら handle_server を呼ぶ
    if (FD_ISSET(STDIN_FILENO, &result_fds)) {
      printf(
          "#################### handle_stdin ####################\n");  // デバッグ用
      if (!handle_stdin(fd, &client, editor)) return 0;
    } else if (FD_ISSET(fd, &result_fds)) {
      printf(
          "#################### handle_server ####################\n");  // デバッグ用
      if (!handle_server(fd, &client, editor, editor->my_client_id)) return 0;
    } else {
      fprintf(stderr, "impossible branch\n");
      return 0;
    }
  }
  return 1;
}

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in sin;

  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = PF_INET;
  parse_args(argc, argv, &sin.sin_addr.s_addr, &sin.sin_port);

  EDITOR editor;

  init_editor(&editor);

  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) /* socket */
    panic("cannot create socket");

  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    panic("cannot connect");

  if (!service(sockfd, &editor)) {
    fprintf(stderr, "client error\n");
  }

  if (shutdown(sockfd, SHUT_RDWR) < 0) panic("cannot shutdown");
  close(sockfd);

  exit_editor(&editor);

  return 0;
}
