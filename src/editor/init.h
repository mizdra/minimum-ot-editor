// # editor ドメイン
//
// ## 用語の定義
// - 行の先頭にある文字
//   - エディタ上で表示されている, 行の一番左側にある文字のこと
//   - ただし改行文字は常に行の末尾にあるものとして処理される
//     -
//     その行が改行文字だけからなる場合はその改行文字が「行の先頭にある文字」であり「行の末尾にある文字」でもある
// - 文字の index
//   - その文字が実際に格納されている `editor->document` の添字のこと
// - 列の総数
//   - `<行にある文字 (改行文字を除く) の総数> + 1`
//   - 行の中でカーソルが動かせる範囲と一致する
// - 行番号/列番号
//   - 1 から始まる行/列に割り当てられた番号のこと
// - 論理的なカーソル
//   - `editor->cursor` が論理的に指し示しているカーソルのこと
// - 物理的なカーソル
//   - 制御文字の出力によってターミナル上に実際に表示されているカーソルのこと

// ## 設計思想
// - 改行, 折返しに対応するため行を扱う関数 `get_head_of_next_row` を用意し,
//   その関数をコアとして行・列の操作を行う
//   - 改行, 折返しに直接対応するロジックが局所化するため,
//   それ以外の場所ではどのように改行, 折返しに対応するかを知らなくて良くなる
// - ドキュメントが空の場合でも 1 行あると見なす
// - カラムは行の中でカーソルが動かせる範囲の数だけあるものとする
//   - 2カーソル分左右に動かせるなら, 2カラムあると判定する
// - エディタはターミナル上に実装される.
//   - 入力は標準入力を, 出力は標準出力を利用する.

#ifndef editor__init_h
#define editor__init_h

#include <termios.h>

#define MAX_DOCUMENT_SIZE 1024

#define DELETE_KEY 127

// ASCIIコード領域 (0 ~ 127) と被らないよう,
// 上下左右キーのコードを割り当てておく
#define ARROW_UP_KEY 128
#define ARROW_DOWN_KEY 129
#define ARROW_RIGHT_KEY 130
#define ARROW_LEFT_KEY 131

// エディタの状態
typedef struct {
  struct termios term;               // エディタ起動中の termios
  struct termios term_bak;           // エディタ起動前の termios
  int cursor;                        // カーソル位置 (Zero-based index)
  char document[MAX_DOCUMENT_SIZE];  // 編集対象の文章
  int my_client_id;
} EDITOR;

// エコーの無効化や非カノニカルモードの有効化などのエディタの初期化を行う.
void init_editor(EDITOR *editor);

// エコーなどの termios の状態を元に戻し, エディタを終了する
void exit_editor(EDITOR *editor);

#endif  // editor__init_h
