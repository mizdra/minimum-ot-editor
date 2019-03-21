#ifndef editor__render_h
#define editor__render_h

#include "editor/init.h"
#include "ot/operation.h"

// beep音を出力する
void beep();

// 画面をクリアし, 物理的なカーソルを先頭に移動する.
// ただし論理的なカーソルの位置は変更しない.
void clear_screen();

// ドキュメントを出力する
void print_document(EDITOR *editor);

// 画面を再描画する
void render_screen(EDITOR *editor);

int apply_action_to_client(EDITOR *editor, ACTION action);

#endif  // editor__render_h
