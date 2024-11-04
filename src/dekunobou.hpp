#include <omp.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "hash.hpp"
#include "option.hpp"

#define perfect_search 13  // 完全読みを始めるタイミング
#define param_size 13125
#define inf 1000000000

#define cur_opening 13122
#define cur_middle 13123
#define cur_ending 13124