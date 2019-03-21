#ifndef editor__cursor_h
#define editor__cursor_h

#include "editor/init.h"

// n + 1 番目の文字の上にカーソルを移動する (Zero-based index).
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_move_by_index(EDITOR* editor, int n);

// row 行 column 列にある文字の上にカーソルを移動する (One-based index).
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_move_by_position(EDITOR* editor, int row, int column);

// カーソルを n 文字分前方に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_forward(EDITOR* editor, int n);

// カーソルを n 文字分後方に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_back(EDITOR* editor, int n);

// カーソルを n 行分上に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_up(EDITOR* editor, int n);

// カーソルを n 行分下に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_down(EDITOR* editor, int n);

// カーソルが n 文字分前方に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_forward(EDITOR* editor, int n);

// カーソルが n 文字分後方に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_back(EDITOR* editor, int n);

// カーソルが n 行分上に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_up(EDITOR* editor, int n);

// カーソルが n 行分下に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_down(EDITOR* editor, int n);

// カーソルの行や列からのはみ出しを修正し,
// 適切な位置にカーソルを移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_reposition(EDITOR* editor);

#endif  // editor__cursor_h
