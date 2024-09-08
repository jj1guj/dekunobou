#include "../../../../../../src/dekunobou.hpp"

#include <cstdint>

#include "../../../../../../src/board.hpp"
#include "../../../../../../src/engine.hpp"
#include "../../../../../../src/eval_param.hpp"

extern "C" {
uint32_t dekunobou(char* board_string, bool turn, uint32_t depth,
                   uint32_t perfect_search_depth);
}

uint32_t dekunobou(char* board_string, bool turn, uint32_t depth,
                   uint32_t perfect_search_depth) {
  // コマンドライン引数をパースしてオプションをセットする
  Option option;
  option.option_web.turn = turn;
  option.option_web.depth = 9;
  option.option_web.perfect_search_depth = 13;

  Board board;

  // 盤をセット
  board.point[0] = 0;
  board.point[1] = 0;
  board.board_black = 0;
  board.board_white = 0;
  for (int i = 0; i < 64; ++i) {
    if (board_string[i] == '1') {
      // 黒石
      board.board_black |=
          (unsigned long long)(1ULL << (unsigned long long)(63 - i));
      ++board.point[0];
    } else if (board_string[i] == '2') {
      // 白石
      board.board_white |=
          (unsigned long long)(1ULL << (unsigned long long)(63 - i));
      ++board.point[1];
    }
  }

  // 手番をセット
  if (!option.option_web.turn) {
    board.turn = false;
    board.board_player = board.board_black;
    board.board_opponent = board.board_white;
  } else {
    board.turn = true;
    board.board_player = board.board_white;
    board.board_opponent = board.board_black;
  }

  return go(board, param, option);
}