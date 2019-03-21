#include "ot/operation.h"

#include <stdio.h>
#include <unistd.h>

// 操作やアクションの型の定義, 補助関数の実装箇所

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
