#ifndef ot__transform_h
#define ot__transform_h

#include "ot/operation.h"

// ##############################
// ### xform
// ##############################

OP_PAIR pair_init(OP left, OP right);

OP_PAIR xform(OP a, OP b);

ACTION_PAIR xform_action(ACTION a, ACTION b);

// ##############################
// ### apply (副作用あり)
// ##############################

int apply_del(char *document, OP op);

int apply_ins(char *document, OP op);

int apply_op(char *document, OP op);

#endif  // ot__transform_h
