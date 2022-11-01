#include<algorithm>
#include<chrono>
#include<cstring>
#include<fstream>
#include<iostream>
#include<istream>
#include<omp.h>
#include<random>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

#pragma once
//モード指定
//#define Debug //デバッグモード
//#define Console //コンソールモードでの対局(default)
#define GA //遺伝的アルゴリズムによる評価関数生成
//#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ

#define perfect_search 13 //完全読みを始めるタイミング
// #define param_size 489
#define param_size 13125
#define inf 1000000000

// #define cur_opening 0
// #define cur_middle 163
// #define cur_ending 326
#define cur_opening 13122
#define cur_middle 13123
#define cur_ending 13124