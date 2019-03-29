#pragma once

#include <stdio.h>
#include <unistd.h>

// 操作やアクションの型の定義, 補助関数の実装箇所

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

OP ins_init(int pos, char text) { return (OP){INS, pos, text}; }

OP del_init(int pos) { return (OP){DEL, pos, '\0'}; }

OP nop_init() { return (OP){NOP, 0, '\0'}; }

ACTION action_init(int operator, int rev, OP op) {
  return (ACTION){operator, rev, op };
}

// ##############################
// ### check
// ##############################

int is_ins(OP op) { return op.type == INS; }

int is_del(OP op) { return op.type == DEL; }

int is_nop(OP op) { return op.type == NOP; }

// ##############################
// ### read/write
// ##############################

void print_op(OP op) {
  if (is_ins(op))
    printf("OP(type=INS, pos=%d, text=%c)", op.pos, op.text);
  else if (is_del(op))
    printf("OP(type=DEL, pos=%d)", op.pos);
  else if (is_nop(op))
    printf("OP(type=NOP)");
  else
    printf("OP(type=%d, pos=%d, text=%c)", op.type, op.pos, op.text);
}

void print_action(ACTION action) {
  printf("ACTION(operator=%d, rev=%d, ", action.operator, action.rev);
  print_op(action.op);
  printf(")");
}

// ##############################
// ### read/write
// ##############################

int read_action(int fd, ACTION *action) {
  if (read(fd, &action->operator, sizeof(int)) <= 0) return 0;
  if (read(fd, &action->rev, sizeof(int)) <= 0) return 0;
  if (read(fd, &action->op, sizeof(OP)) <= 0) return 0;

  // デバッグ用
  printf("receive action: ");
  print_action(*action);
  printf("\n");
  return 1;
}

int write_action(int fd, ACTION action) {
  if (write(fd, &action.operator, sizeof(int)) < 0) return 0;
  if (write(fd, &action.rev, sizeof(int)) < 0) return 0;
  if (write(fd, &action.op, sizeof(OP)) < 0) return 0;

  // デバッグ用
  printf("send action: ");
  print_action(action);
  printf("\n");
  return 1;
}
