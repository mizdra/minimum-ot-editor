#pragma once

#include <stdio.h>

#include "common/shared.h"
#include "ot/operation.h"
#include "ot/transform.h"

// クライアントのステータス
typedef enum {
  // クライアントの操作がサーバに適用されていることがクライアントから確認されている状態.
  SYNCHRONIZED,
  // クライアントから,
  // クライアントの操作がまだサーバに適用されていることが確認できていない状態.
  AWAITING,
} CLIENT_STATE;

// クライアントの状態
typedef struct {
  int rev;  // クライアントのドキュメントのリビジョン
  CLIENT_STATE state;              // クライアントの状態
  ACTION outstanding;              // 送信中のアクション
  int buffer_size;                 // バッファのサイズ
  ACTION buffer[MAX_ACTION_SIZE];  // 送信待機中のアクション
} CLIENT;

// クライアントの状態を初期化する
void client_init(CLIENT *client, int rev) {
  client->rev = rev;
  client->state = SYNCHRONIZED;
  client->buffer_size = 0;
}

// バッファにアクションを追加する
void push_action_from_buffer(CLIENT *client, ACTION action) {
  client->buffer[client->buffer_size] = action;
  client->buffer_size++;
}

// バッファから最も古いアクションを取り出す
ACTION shift_action_from_buffer(CLIENT *client) {
  ACTION result = client->buffer[0];
  int i;
  for (i = 1; i < client->buffer_size; i++) {
    client->buffer[i - 1] = client->buffer[i];
  }
  client->buffer_size--;
  return result;
}

// デバッグ用にクライアントのステータスやバッファの状態を出力する
void print_state(CLIENT *client) {
  if (client->state == SYNCHRONIZED) {
  } else {
    printf("  outstanding:\n    ");
    print_action(client->outstanding);
    printf("\n  buffer(size=%d):\n", client->buffer_size);
    int i;
    for (i = 0; i < client->buffer_size; i++) {
      printf("    ");
      print_action(client->buffer[i]);
      printf("\n");
    }
  }
}

// サーバからアクションを受け取り, 操作変換してクライアントに適用可能な形に
// したものを action に格納する
int recv_action_from_server(CLIENT *client, int fd, int my_client_id,
                            ACTION *action) {
  ACTION received_action;

  // 本来は受信した action が不正なものでないかチェックする必要があるが,
  // 今回は必ず適用可能な形へと変換できる action が受信されると仮定する
  if (!read_action(fd, &received_action)) {
    fprintf(stderr, "fail to read_action\n");
    return 0;
  }

  // 自分が送信した操作が返ってきた時
  if (received_action.operator== my_client_id) {
    if (client->buffer_size == 0) {
      // バッファが空なら SYNCHRONIZED へと復帰する
      client->state = SYNCHRONIZED;
      printf("[AWAITING] -> [SYNCHRONIZED]\n");
    } else {
      // バッファが残ってるならバッファから最も古いアクションを取り出し,
      // それをサーバに送信する
      ACTION buffered_action = shift_action_from_buffer(client);
      if (!write_action(fd, buffered_action)) {
        fprintf(stderr, "fail to write_action\n");
        return 0;
      }
      client->outstanding = buffered_action;
      printf("[AWAITING] -> [AWAITING] (pop)\n");
      print_state(client);
    }
    // 自分が送信したアクションは既に送信時に適用済みなので,
    // この場合は何も変更しない nop を返す
    *action =
        action_init(received_action.operator, received_action.rev, nop_init());
    return 1;
  }

  if (client->state == SYNCHRONIZED) {
    // SYNCHRONIZED ならサーバとクライアントのリビジョンが一致しているので,
    // 操作変換せずに received_action をそのまま適用可能なアクションとして
    // action に格納する
    *action = received_action;
  } else {
    // AWAITING
    // ならクライアントのリビジョンがサーバのリビジョンよりも進んでいるので,
    // サーバから受信したアクションをクライアントに適用可能な形に操作変換してから,
    // 適用可能なアクションとして action に格納する
    ACTION_PAIR action_pair =
        xform_action(client->outstanding, received_action);
    client->outstanding = action_pair.right;
    int i;
    for (i = 0; i < client->buffer_size; i++) {
      action_pair = xform_action(client->buffer[i], action_pair.left);
      client->buffer[i] = action_pair.right;
    }
    *action = action_pair.left;
  }

  // 新たなアクションを適用してドキュメントが変更されるので
  // リビジョンをインクリメントする
  client->rev++;
  return 1;
}

// サーバにアクションを送信する
int send_action_to_server(CLIENT *client, int fd, ACTION action) {
  // 送信するアクションがクライアントのリビジョンと一致しなければエラー
  if (action.rev != client->rev) return 0;

  if (client->state == SYNCHRONIZED) {
    // SYNCHRONIZED ならそのままサーバにアクションを送信し,
    // ステータスを AWAITING にする
    if (!write_action(fd, action)) return 0;
    client->state = AWAITING;
    client->outstanding = action;
    client->buffer_size = 0;

    // デバッグ用
    printf("[SYNCHRONIZED] -> [AWAITING]\n");
    print_state(client);
  } else {
    // AWAITING ならバッファにアクションを追加し, 送信を予約する.
    // (それよりも前に送信が予約されたアクションが全て送信しきれた時に送られる)
    push_action_from_buffer(client, action);

    // デバッグ用
    printf("[AWAITING] -> [AWAITING] (push)\n");
    print_state(client);
  }

  // 新たなアクションを適用してドキュメントが変更されるので
  // リビジョンをインクリメントする
  client->rev++;

  return 1;
}
