#ifndef ot__server_h
#define ot__server_h

#include <sys/select.h>

#include "common/shared.h"
#include "ot/operation.h"

// サーバの状態
typedef struct {
  int rev;  // サーバのドキュメントのリビジョン. history_size と一致.
  ACTION history[MAX_ACTION_SIZE];
} SERVER;

// サーバの状態を初期化する関数
void server_init(SERVER *server, int rev);

// action をサーバに適用可能な形へと操作変換したものを action_dash に格納する
int reduce_action(SERVER *server, ACTION action, ACTION *action_dash);

// 履歴を更新する
void push_history(SERVER *server, ACTION action);

// 全クライアントにアクションを送信する
void broadcast_action_to_clients(fd_set *client_fds, ACTION action);

// クライアントからアクションを受け取り, 操作変換してサーバに適用可能な形に
// したものを action に格納する
int recv_action_from_client(SERVER *server, fd_set *client_fds, int client_fd,
                            ACTION *action);

#endif  // ot__server_h
