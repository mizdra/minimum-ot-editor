// 操作やアクションの型の定義, 補助関数の実装箇所

#ifndef ot__operation_h
#define ot__operation_h

typedef enum {
  INS,  // 文字の挿入を表す操作
  DEL,  // 文字の削除を表す操作
  NOP,  // 何もしない操作
} OP_TYPE;

typedef struct {
  OP_TYPE type;
  int pos;    // 操作対象の文字のドキュメント上での位置
  char text;  // 挿入する文字 (INS 専用のフィールド)
} OP;

// クライアント, サーバ間で送り合う操作を表す構造体 (アクション).
typedef struct {
  int operator;  // 操作を発行したクライアントの id
  int rev;       // 操作が適用可能なドキュメントのリビジョン
  OP op;         // 操作
} ACTION;

typedef struct {
  OP left;
  OP right;
} OP_PAIR;

typedef struct {
  ACTION left;
  ACTION right;
} ACTION_PAIR;

// ##############################
// ### init
// ##############################

OP ins_init(int pos, char text);

OP del_init(int pos);

OP nop_init();

ACTION action_init(int operator, int rev, OP op);

// ##############################
// ### check
// ##############################

int is_ins(OP op);

int is_del(OP op);

int is_nop(OP op);

// ##############################
// ### read/write
// ##############################

void print_op(OP op);

void print_action(ACTION action);

// ##############################
// ### read/write
// ##############################

int read_action(int fd, ACTION *action);

int write_action(int fd, ACTION action);

#endif  // ot__operation_h
