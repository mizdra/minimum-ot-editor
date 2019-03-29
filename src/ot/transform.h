#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ot/operation.h"

// ##############################
// ### xform
// ##############################

OP_PAIR pair_init(OP left, OP right) { return (OP_PAIR){left, right}; }

OP_PAIR xform(OP a, OP b) {
  // */nop
  if (is_nop(a) && is_nop(b)) return pair_init(nop_init(), nop_init());
  if (is_nop(a) && is_ins(b)) return pair_init(b, nop_init());
  if (is_ins(a) && is_nop(b)) return pair_init(nop_init(), a);
  if (is_nop(a) && is_del(b)) return pair_init(b, nop_init());
  if (is_del(a) && is_nop(b)) return pair_init(nop_init(), a);

  // del/del
  if (is_del(a) && is_del(b)) {
    if (a.pos == b.pos) return pair_init(nop_init(), nop_init());
    if (a.pos > b.pos)
      return pair_init(del_init(b.pos), del_init(a.pos - 1));
    else
      return pair_init(del_init(b.pos - 1), del_init(a.pos));
  }

  // ins/ins
  if (is_ins(a) && is_ins(b)) {
    if (a.pos == b.pos) {
      if (a.text == b.text) return pair_init(nop_init(), nop_init());
      if (a.text < b.text)
        return pair_init(ins_init(a.pos + 1, b.text), ins_init(a.pos, a.text));
      else
        return pair_init(ins_init(a.pos, b.text), ins_init(a.pos + 1, a.text));
    }
    if (a.pos > b.pos)
      return pair_init(ins_init(b.pos, b.text), ins_init(a.pos + 1, a.text));
    else
      return pair_init(ins_init(b.pos + 1, b.text), ins_init(a.pos, a.text));
  }

  // ins/del
  if (is_ins(a) && is_del(b)) {
    if (a.pos <= b.pos)
      return pair_init(del_init(b.pos + 1), ins_init(a.pos, a.text));
    else
      return pair_init(del_init(b.pos), ins_init(a.pos - 1, a.text));
  }
  if (is_del(a) && is_ins(b)) {
    OP_PAIR pair = xform(b, a);
    return pair_init(pair.right, pair.left);
  }

  fprintf(stderr, "Unknown operation type. ((a.type, b.type) == (%d, %d))\n",
          a.type, b.type);
  exit(1);
}

ACTION_PAIR xform_action(ACTION a, ACTION b) {
  if (a.rev != b.rev) {
    fprintf(stderr,
            "Actions with different revisions can not be merged. ((a.rev, "
            "b.rev) == (%d, %d))\n",
            a.rev, b.rev);
    exit(1);
  }
  OP_PAIR op_pair = xform(a.op, b.op);

  return (ACTION_PAIR){
      action_init(b.operator, a.rev + 1, op_pair.left),  // b_dash
      action_init(a.operator, b.rev + 1, op_pair.right)  // a_dash
  };
}

// ##############################
// ### apply (副作用あり)
// ##############################

int apply_del(char *document, OP op) {
  if (op.pos < 0 || (int)strlen(document) <= op.pos) return 0;

  // shift left
  int p = op.pos;
  while (document[p] != '\0') {
    document[p] = document[p + 1];
    p++;
  }

  return 1;
}

int apply_ins(char *document, OP op) {
  if (op.pos < 0 || (int)strlen(document) < op.pos) return 0;

  // shift right

  // document[strlen(document)] に格納されている NULL文字も
  // 一緒に shift する
  int p = strlen(document);
  while (p >= op.pos) {
    document[p + 1] = document[p];
    p--;
  }
  document[op.pos] = op.text;
  return 1;
}

int apply_op(char *document, OP op) {
  if (is_nop(op)) return 1;
  if (is_del(op)) return apply_del(document, op);
  if (is_ins(op)) return apply_ins(document, op);
  return 0;
}
