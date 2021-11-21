#include "dekunobou.hpp"
#include "board.hpp"
#include "legalmovelist.hpp"

/**********paramについて************/
//新しい評価関数の仕様
/*
これを参考にする
https://qiita.com/na-o-ys/items/10d894635c2a6c07ac70
角周辺の石の配置は4桁の3進数とみなしてこれを10進数に変換し, 対応するインデックスから値を引っ張ってくる
a*盤上にしめる自石の割合+辺の形の評価値の合計
なにもない: 0
先手の石: 1
後手の石: 2
これで先手目線にして後手番で評価するときには正負を反転させる
例
石の配置: *ox
1*2+3*1+9*0=5
0~80: 辺の形の評価値(序盤)
81~161: 斜めの形の評価値(序盤)
162: a(序盤)
163~243: 辺の形の評価値(中盤)
244~324: 斜めの形の評価値(中盤)
325: a(中盤)
326~406: 辺の形の評価値(終盤)
407~487: 斜めの形の評価値(終盤)
488: a(終盤)
*/

float calc_shape_value(Board& board,float param[param_size],int cur_offset);
float eval(Board board,float param[param_size]);