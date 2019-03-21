#ifndef editor__window_h
#define editor__window_h

#include "editor/init.h"

// エディタのwindowサイズを取得する
int get_window_width();

// 行の先頭にある文字の index を受け取り,
// その次の行の先頭にある文字の index を返す.
// 次の行が存在すれば index を, そうでなければ偽を返す.
int get_head_of_next_row(EDITOR *editor, int head_of_row);

// row 行目の先頭にある文字の index を返す
int get_head_of_row(EDITOR *editor, int row);

// 行の総数を返す
int get_row_count(EDITOR *editor);

// row 行目の列の総数を返す.
int get_column_count(EDITOR *editor, int row);

// 論理的なカーソルが指し示す行番号を返す
int get_current_row(EDITOR *editor);

// 論理的なカーソルが指し示す列番号を返す
int get_current_column(EDITOR *editor);

#endif  // editor__window_h
