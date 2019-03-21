#ifndef editor__input_h
#define editor__input_h

#include "editor/init.h"
#include "ot/operation.h"

// 印字可能なASCII文字, または矢印キーを読み込む.
// 読み込みに成功したら真, 失敗したら偽を返す.
int scan_key();

OP beep_and_nop();

// operation を読み込む.
// 読み込みに成功したら真, 失敗したら偽を返す.
OP scan_operation(EDITOR *editor);

#endif  // editor__input_h
