#include "eval.hpp"

/**********paramについて************/
// 新しい評価関数の仕様
/*
これを参考にする
https://qiita.com/na-o-ys/items/10d894635c2a6c07ac70
角周辺の石の配置は4桁の3進数とみなしてこれを10進数に変換し,
対応するインデックスから値を引っ張ってくる
a*盤上にしめる自石の割合+辺の形の評価値の合計
なにもない: 0
先手の石: 1
後手の石: 2
これで先手目線にして後手番で評価するときには正負を反転させる
例
石の配置: *ox
1*2+3*1+9*0=5
0~6560: 辺の形の評価値
6561~13121: 斜めの形の評価値
13122: a(序盤)
13123: a(中盤)
13124: a(終盤)
*/
constexpr int param_cur[3] = {cur_opening, cur_middle, cur_ending};
// 角周辺の形を計算するときに使用
constexpr int shape_ref[6][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7},        {0, 8, 16, 24, 32, 40, 48, 56},
    {7, 15, 23, 31, 39, 47, 55, 63}, {56, 57, 58, 59, 60, 61, 62, 63},
    {0, 9, 18, 27, 36, 45, 54, 63},  // 斜め
    {7, 14, 21, 28, 35, 42, 49, 56}, // 斜め
};

constexpr int pow3[8] = {1, 3, 9, 27, 81, 243, 729, 2187};

// 角付近の形を評価する
float calc_shape_value(Board &board, float param[param_size], int cur_offset) {
  float val = 0;
  int index;
  int ref, ref_value;
  // 角付近の形
  for (int i = 0; i < 6; ++i) {
    index = 0;
    for (int j = 0; j < 8; ++j) {
      ref = shape_ref[i][j];
      ref_value = (board[ref] + 3) % 3;
      index += pow3[j] * ref_value;
    }

    if (i <= 3)
      val += param[index];
    else
      val += param[index + 6561];
  }
  // 後手番のときは符号を反転
  if (board.turn)
    val *= -1.0;
  return val;
}

// 評価値の計算(手番側が有利ならプラス)
float eval(Board &board, float param[param_size]) {
  int cur_offset = param_cur[board.ply / 20];
  float ans =
      6.0 * board.point[!board.turn] / (board.point[0] + board.point[1]);
  // ans*=param[cur_offset+81];
  ans *= param[cur_offset];
  ans += calc_shape_value(board, param, cur_offset);
  return ans;
}
