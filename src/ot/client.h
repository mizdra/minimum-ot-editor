#ifndef ot__client_h
#define ot__client_h

#include "common/shared.h"
#include "ot/operation.h"

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
void client_init(CLIENT *client, int rev);

// バッファにアクションを追加する
void push_action_from_buffer(CLIENT *client, ACTION action);

// バッファから最も古いアクションを取り出す
ACTION shift_action_from_buffer(CLIENT *client);

// デバッグ用にクライアントのステータスやバッファの状態を出力する
void print_state(CLIENT *client);

// サーバからアクションを受け取り, 操作変換してクライアントに適用可能な形に
// したものを action に格納する
int recv_action_from_server(CLIENT *client, int fd, int my_client_id,
                            ACTION *action);

// サーバにアクションを送信する
int send_action_to_server(CLIENT *client, int fd, ACTION action);

#endif  // ot__client_h
