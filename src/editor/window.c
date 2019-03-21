#include "editor/window.h"

#include <sys/ioctl.h>
#include <unistd.h>

// エディタのwindowサイズを取得する
int get_window_width() {
  // 今回エディタはターミナル上に実装されているので,
  // ターミナルの画面サイズを取得する
  struct winsize wins;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &wins);
  return wins.ws_col;
}

// 行の先頭にある文字の index を受け取り,
// その次の行の先頭にある文字の index を返す.
// 次の行が存在すれば index を, そうでなければ偽を返す.
int get_head_of_next_row(EDITOR *editor, int head_of_row) {
  int win_width = get_window_width();

  int n = head_of_row;
  int column_count = 1;
  while (1) {
    if (editor->document[n] == '\0') return 0;
    if (editor->document[n] == '\n') {
      int new_head = n + 1;  // 改行をスキップ
      if (editor->document[new_head] == '\0') return 0;
      return new_head;
    }

    // editor->document[n] には何らかの文字が存在する
    if (column_count > win_width) return n;
    column_count++;
    n++;
  }
}

// row 行目の先頭にある文字の index を返す
int get_head_of_row(EDITOR *editor, int row) {
  int row_head = 0;
  int r = 1;
  while (r < row) {
    row_head = get_head_of_next_row(editor, row_head);
    r++;
  }
  return row_head;
}

// 行の総数を返す
int get_row_count(EDITOR *editor) {
  int row_head = 0;

  int row = 1;
  while (1) {
    row_head = get_head_of_next_row(editor, row_head);
    if (!row_head) return row;
    row++;
  }
  return -1;  // unreachable code
}

// row 行目の列の総数を返す.
int get_column_count(EDITOR *editor, int row) {
  int win_width = get_window_width();
  int column_count = 1;
  int n = get_head_of_row(editor, row);
  while (editor->document[n] != '\0' && editor->document[n] != '\n' &&
         column_count < win_width) {
    column_count++;
    n++;
  }
  return column_count;
}

// 論理的なカーソルが指し示す行番号を返す
int get_current_row(EDITOR *editor) {
  int row = 1;
  int row_head = 0;

  while (1) {
    row_head = get_head_of_next_row(editor, row_head);
    // row 行が最後であればそれが current_row
    if (!row_head) return row;
    // row + 1 行がカーソルよりも先であれば, row が current_row
    if (row_head > editor->cursor) return row;
    row++;
  }
}

// 論理的なカーソルが指し示す列番号を返す
int get_current_column(EDITOR *editor) {
  int row_head = get_head_of_row(editor, get_current_row(editor));
  return editor->cursor - row_head + 1;
}
