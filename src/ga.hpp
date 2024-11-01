#include <cmath>
#include <set>

#include "play.hpp"
#pragma once
#define N 1024
/****ここから関数群****/
void init_param(float params[param_size]);

// 遺伝的アルゴリズムで使用したパラメータすべてをCSVファイルから読み込む
int load_params(std::string filename, float params[N][param_size]);

// 遺伝的アルゴリズムで生成したパラメータをすべてCSVファイルに格納する
void out_params(std::string path);

// 交叉
void intersection(float p1[param_size], float p2[param_size], int cur1,
                  int cur2);

void ga(const Option &option);