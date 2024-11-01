#include "board.hpp"
#pragma once
class LegalMoveList {
 public:
  LegalMoveList() {}
  LegalMoveList(Board board) {
    unsigned long long legalBoard =
        makeLegalBoard(board.board_player, board.board_opponent);
    move_num = 0;
    unsigned long long id;
    for (int i = 0; i < 64; ++i) {
      id = board.idToBit(i);
      if ((id & legalBoard) == id) {
        movelist[move_num] = i;
        ++move_num;
      }
    }
  }

  int size() { return move_num; }

  // 添字で合法手を取得
  int &operator[](int i) { return this->movelist[i]; }

 private:
  int movelist[64];
  int move_num;
};