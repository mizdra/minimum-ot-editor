#pragma once

#include <stdio.h>
#include <string.h>

#include "common/util.h"
#include "editor/window.h"

// n + 1 番目の文字の上にカーソルを移動する (Zero-based index).
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_move_by_index(EDITOR* editor, int n) {
  if (n < 0) {
    print_err("cursor index must be 0 or above: (n = %d)", n);
    return 0;
  }

  editor->cursor = n;
  int row = get_current_row(editor);
  int column = get_current_column(editor);

  printf("\033[%d;%dH", row, column);
  return 1;
}

// row 行 column 列にある文字の上にカーソルを移動する (One-based index).
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_move_by_position(EDITOR* editor, int row, int column) {
  if (row < 1 || column < 1) {  // row が小さすぎる場合
    print_err("row is too small: (row, column) = (%d, %d)", row, column);
    return 0;
  }
  if (column < 1) {  // column が小さすぎる場合
    print_err("column is too small: (row, column) = (%d, %d)", row, column);
    return 0;
  }

  // 新しいカーソルの位置を計算する
  int row_head = 0, r = 1;
  while (r < row) {
    row_head = get_head_of_next_row(editor, row_head);
    if (!row_head) {  // row が大きすぎる場合
      print_err("row is too big: (row, column) = (%d, %d)", row, column);
      return 0;
    }
    r++;
  }
  if (column > get_column_count(editor, row)) {  // column が大きすぎる場合
    print_err("column is too big: (row, column) = (%d, %d)", row, column);
    return 0;
  }
  int new_cursor = row_head + column - 1;

  printf("\033[%d;%dH", row, column);
  editor->cursor = new_cursor;
  return 1;
}

// カーソルを n 文字分前方に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_forward(EDITOR* editor, int n) {
  return cursor_move_by_index(editor, editor->cursor + n);
}

// カーソルを n 文字分後方に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_back(EDITOR* editor, int n) {
  return cursor_move_by_index(editor, editor->cursor - n);
}

// カーソルを n 行分上に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_up(EDITOR* editor, int n) {
  int next_row = get_current_row(editor) - n;
  int next_column =
      min(get_current_column(editor), get_column_count(editor, next_row));
  return cursor_move_by_position(editor, next_row, next_column);
}

// カーソルを n 行分下に移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_down(EDITOR* editor, int n) {
  int next_row = get_current_row(editor) + n;
  int next_column =
      min(get_current_column(editor), get_column_count(editor, next_row));
  return cursor_move_by_position(editor, next_row, next_column);
}

// カーソルが n 文字分前方に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_forward(EDITOR* editor, int n) {
  return editor->cursor + n <= (int)strlen(editor->document);
}

// カーソルが n 文字分後方に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_back(EDITOR* editor, int n) { return editor->cursor - n >= 0; }

// カーソルが n 行分上に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_up(EDITOR* editor, int n) {
  return get_current_row(editor) - n >= 1;
}

// カーソルが n 行分下に移動可能かどうかをチェックする.
// 移動可能なら真, そうでなければ偽を返す.
int can_cursor_down(EDITOR* editor, int n) {
  return get_current_row(editor) + n <= get_row_count(editor);
}

// カーソルの行や列からのはみ出しを修正し,
// 適切な位置にカーソルを移動する.
// 移動に成功したら真を, 失敗したら偽を返す.
int cursor_reposition(EDITOR* editor) {
  if (editor->cursor < 0)
    return cursor_move_by_index(editor, 0);
  else if (editor->cursor > (int)strlen(editor->document))
    return cursor_move_by_index(editor, strlen(editor->document));
  else
    return cursor_move_by_index(editor, editor->cursor);
}
