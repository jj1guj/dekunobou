#include<iostream>
#include<algorithm>
#include<random>
#include<string>
#include<vector>
#include<fstream>
#include<istream>
#include<omp.h>
#include<sstream>

#pragma once
//モード指定
#define Debug //デバッグモード
#define Console //コンソールモードでの対局(default)
#define GA //遺伝的アルゴリズムによる評価関数生成
//#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ

#define perfect_search 11 //完全読みを始めるタイミング
#define param_size 489
#define inf 1000000000

#define cur_opening 0
#define cur_middle 163
#define cur_ending 326