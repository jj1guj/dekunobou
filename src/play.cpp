#include "play.hpp"

// エンジン同士の対局
int play_engine(float param_black[param_size], float param_white[param_size]) {
  Board board;
  int pass_count = 0;
  int move;
  while (true) {
#if defined Debug
    // for debug
    disp(board);
#endif
    if (pass_count > 1)
      break;
    LegalMoveList moves(board);

    if (moves.size() == 0) {
      board.push(-1);
      ++pass_count;
      continue;
    }
    pass_count = 0;
#if defined Debug
    disp_teban(board);
#endif
    if (board.turn) {
      // 後手番
      move = go(board, param_white);
    } else {
      // 先手番
      move = go(board, param_black);
    }
    // 着手
    board.push(move);
  }

#if not defined GA
  disp(board);
  std::cout << "black: " << board.point[0] << "vs" << board.point[1]
            << " :white\n";
#endif
  if (board.point[0] > board.point[1])
    return 0;
  else if (board.point[0] < board.point[1])
    return 1;
  else
    return 2;
}