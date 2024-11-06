#include "play.hpp"

// エンジン同士の対局
int play_engine(int param_black[param_size], int param_white[param_size],
                const Option &option) {
  Board board;
  int pass_count = 0;
  int move;
  while (true) {
    if (option.debug) disp(board);
    if (pass_count > 1) break;
    LegalMoveList moves(board);

    if (moves.size() == 0) {
      board.push(-1);
      ++pass_count;
      continue;
    }
    pass_count = 0;
    if (option.debug) disp_teban(board);
    if (board.turn) {
      // 後手番
      move = go(board, param_white, option);
    } else {
      // 先手番
      move = go(board, param_black, option);
    }
    // 着手
    board.push(move);
  }

  if (option.mode != Mode::ga) {
    disp(board);
    std::cout << "black: " << board.point[0] << "vs" << board.point[1]
              << " :white\n";
  }

  if (board.point[0] > board.point[1])
    return 0;
  else if (board.point[0] < board.point[1])
    return 1;
  else
    return 2;
}