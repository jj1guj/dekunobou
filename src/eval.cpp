#include "eval.hpp"

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

//角周辺の形を計算するときに使用
int shape_ref[12][4]={
    {0,1,2,3},
    {0,8,16,24},
    {7,6,5,4},
    {7,15,22,29},
    {56,48,40,32},
    {56,57,58,59},
    {63,62,61,60},
    {63,55,47,39},
    {0,9,18,27},//斜め
    {7,14,21,28},//斜め
    {56,49,42,35},//斜め
    {63,54,45,36}//斜め
};

//角付近の形を評価する
float calc_shape_value(Board& board,float param[param_size],int cur_offset){
    float val=0;
    int index;
    int ref1,ref2,ref3,ref4;
    //角付近の形
    for(int i=0;i<12;++i){
        ref1=shape_ref[i][0];ref2=shape_ref[i][1];ref3=shape_ref[i][2];ref4=shape_ref[i][3];
        index=(board[ref1]+3)%3;
        index+=3*(board[ref2]+3)%3;
        index+=9*(board[ref3]+3)%3;
        index+=27*(board[ref4]+3)%3;
        val+=param[index+cur_offset];
    }
    //後手番のときは符号を反転
    if(!board.turn)val*=-1.0;
    return val;
}

//評価値の計算(手番側が有利ならプラス)
float eval(Board board,float param[param_size],int cur_offset){
    float ans=12.0*board.point[!board.turn]/(board.point[0]+board.point[1]);
    ans*=param[cur_offset+81];
    ans+=calc_shape_value(board,param,cur_offset);
    return ans;
}
