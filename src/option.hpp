#include <algorithm>
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <regex>
#include <string>
#pragma once

#define OPT_STRING "m:M:b:t:dhe:"

static struct option long_options[] = {
    {"mode", required_argument, NULL, 'm'},
    {"match_genetic", required_argument, NULL, 0},
    {"thresh", required_argument, NULL, 0},
    {"mutation_start", required_argument, NULL, 0},
    {"mutation_prob", required_argument, NULL, 0},
    {"time_limit", required_argument, NULL, 0},
    {"thread", required_argument, NULL, 0},
    {"out_path", required_argument, NULL, 0},
    {"board", required_argument, NULL, 'b'},
    {"turn", required_argument, NULL, 't'},
    {"eval", required_argument, NULL, 'e'},
    {"debug", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'}};

enum class option_status_t {
  success,
  help,
  invalid_argument,
  not_number,
  path_not_found,
  out_of_range,
  mode_not_selected,
  turn_not_selected,
  path_not_selected,
  board_not_set,
  not_directory,
  not_file,
};

enum class Mode {
  none,
  ga,
  web,
};

struct Option_ga {
  int M = 100;
  int match_genetic = 30;
  float thresh = 0.72;
  int mutation_start = 0;
  float mutation_prob = 1e-3;
  int time_limit = 36;
  int thread = 10;
  std::string out_path = "";
};

struct Option_web {
  std::string board;
  bool turn;
  bool is_turn_set = false;
  std::string eval = "";
};

struct Option {
  Mode mode = Mode::none;
  bool debug = false;
  Option_ga option_ga;
  Option_web option_web;
};

int set_option(int argc, char **argv, Option &option);