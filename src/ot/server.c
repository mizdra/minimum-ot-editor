#include "ot/server.h"

#include <stdio.h>

#include "ot/transform.h"

// サーバの状態を初期化する関数
void server_init(SERVER *server, int rev) { server->rev = rev; }

// action をサーバに適用可能な形へと操作変換したものを action_dash に格納する
int reduce_action(SERVER *server, ACTION action, ACTION *action_dash) {
  // アクションがサーバに存在しないリビジョンを起点としていた場合, エラー
  if (action.rev < 0 || server->rev < action.rev) {
    fprintf(stderr,
            "operation revision not in history: (action.rev, server->rev) = "
            "(%d, %d)\n",
            action.rev, server->rev);
    return 0;
  }

  // 同じリビジョンを持つ履歴上のアクションに対し, xform を適用していく
  *action_dash = action;
  int i;
  for (i = action.rev; i < server->rev; i++) {
    *action_dash = xform_action(*action_dash, server->history[i]).right;
  }
  return 1;
}

// 履歴を更新する
void push_history(SERVER *server, ACTION action) {
  server->history[server->rev++] = action;
}

// 全クライアントにアクションを送信する
void broadcast_action_to_clients(fd_set *client_fds, ACTION action) {
  int fd;
  for (fd = 0; fd < FD_SETSIZE; fd++) {
    if (!FD_ISSET(fd, client_fds))
      continue;  // fdに対応するクライアントがなければスキップ
    if (!write_action(fd, action)) {
      fprintf(stderr, "WARNING: fail to send action to client: client_id=%d\n",
              fd);
    }
  }
}

// クライアントからアクションを受け取り, 操作変換してサーバに適用可能な形に
// したものを action に格納する
int recv_action_from_client(SERVER *server, fd_set *client_fds, int client_fd,
                            ACTION *action) {
  ACTION received_action;
  if (!read_action(client_fd, &received_action)) {
    fprintf(stderr, "fail to read_action\n");
    return 0;
  }

  if (!reduce_action(server, received_action, action)) {
    fprintf(stderr, "fail to reduce_action\n");
    return 0;
  }

  push_history(server, *action);

  broadcast_action_to_clients(client_fds, *action);
  return 1;
}
