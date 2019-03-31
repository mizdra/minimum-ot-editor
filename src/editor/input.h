#pragma once

#include <stdio.h>

#include "editor/cursor.h"
#include "editor/init.h"
#include "editor/render.h"
#include "editor/window.h"
#include "ot/operation.h"

// 印字可能なASCII文字, または矢印キーを読み込む.
// 読み込みに成功したら真, 失敗したら偽を返す.
int scan_key() {
  int ascii = fgetc(stdin);
  if (ascii != '\033') return ascii;  // エスケープ文字でなければそのまま返す

  // Control Sequence Introducer でなければエラーを出力し, リトライする
  if (fgetc(stdin) != '[') {
    print_err("unrecognized key");
    scan_key();
  }

  switch ((ascii = fgetc(stdin))) {
    case 'A':
      return ARROW_UP_KEY;
    case 'B':
      return ARROW_DOWN_KEY;
    case 'C':
      return ARROW_RIGHT_KEY;
    case 'D':
      return ARROW_LEFT_KEY;
  }

  // サポートしていないキーはエラーを出力し, リトライする
  print_err("unrecognized key");
  scan_key();
  return 0;
}

OP beep_and_nop() {
  beep();
  return nop_init();
}

// operation を読み込む.
// 読み込みに成功したら真, 失敗したら偽を返す.
OP scan_operation(EDITOR *editor) {
  int key = scan_key();
  switch (key) {
    case ARROW_UP_KEY:
      if (!can_cursor_up(editor, 1)) return beep_and_nop();
      cursor_up(editor, 1);
      return nop_init();
    case ARROW_DOWN_KEY:
      if (!can_cursor_down(editor, 1)) return beep_and_nop();
      cursor_down(editor, 1);
      return nop_init();
    case ARROW_RIGHT_KEY:
      if (!can_cursor_forward(editor, 1)) return beep_and_nop();
      cursor_forward(editor, 1);
      return nop_init();
    case ARROW_LEFT_KEY:
      if (!can_cursor_back(editor, 1)) return beep_and_nop();
      cursor_back(editor, 1);
      return nop_init();
    case DELETE_KEY:
      if (editor->cursor == 0) return beep_and_nop();
      return del_init(editor->cursor - 1);
    default:
      return ins_init(editor->cursor, (char)key);
  }
}
