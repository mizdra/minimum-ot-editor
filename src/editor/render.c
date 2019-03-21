#include "editor/render.h"

#include <stdio.h>

#include "editor/cursor.h"
#include "ot/transform.h"

// beep音を出力する
void beep() {
  // beep音を表す制御文字 ("\077") を出力することで
  // カーソル位置が移動しないよう, beep音の出力の前後で
  // カーソル位置の保存 ("\033[s") ・復元 ("\033[u") を行う.
  printf("\033[s\007\033[u");
}

// 画面をクリアし, 物理的なカーソルを先頭に移動する.
// ただし論理的なカーソルの位置は変更しない.
void clear_screen() {
  printf("\033[2J\033[0;0H");
  // cursor_top(editor);
}

// ドキュメントを出力する
void print_document(EDITOR *editor) { printf("%s\n", editor->document); }

// 画面を再描画する
void render_screen(EDITOR *editor) {
  // ドキュメントを更新
  clear_screen();
  print_document(editor);

  // ドキュメントを更新した後にカーソルの位置を修正する
  cursor_reposition(editor);

  // 描画結果を即座に反映させるため, printf 内部のバッファをflush
  fflush(stdout);
}

int apply_action_to_client(EDITOR *editor, ACTION action) {
  OP op = action.op;

  // 文字の挿入/削除を適用する
  if (!apply_op(editor->document, op)) return 0;

  // operation の種類に応じてカーソルを移動する
  if (is_ins(op)) {
    if (op.pos <= editor->cursor) cursor_forward(editor, 1);
  } else if (is_del(op)) {
    if (op.pos < editor->cursor) cursor_back(editor, 1);
  }

  render_screen(editor);
  return 1;
}
