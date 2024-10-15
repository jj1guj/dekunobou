#include <iostream>
#pragma once
unsigned long long makeLegalBoard(unsigned long long board_player,
                                  unsigned long long board_opponent);

class Board {
public:
  bool turn; // 後手のときtrue
  int ply;   // 今何手目か
  int point[2];
  unsigned long long board_player;
  unsigned long long board_opponent;
  unsigned long long board_black;
  unsigned long long board_white;
  struct Hash;

  Board() {
    turn = false; // 後手のときtrue
    ply = 1;      // 今何手目か
    point[0] = 2;
    point[1] = 2;
    board_player = 0x0000000810000000;
    board_opponent = 0x0000001008000000;
    board_black = 0x0000000810000000;
    board_white = 0x0000001008000000;
  }

  // 指定した座標に何色の石がおいてあるか返す
  int operator[](int i);

  // 座標をビットに変換
  unsigned long long idToBit(int id);

  // 着手できるか判定
  bool putable(unsigned long long id);

  // 着手
  void push(int id_int);

  // パスの判定
  bool is_pass();

  // 終局の判定
  bool is_over();

  // 手番交代
  void swapBoard();

  bool operator==(const Board& rhs) const;
  bool operator!=(const Board& rhs) const;

private:
  unsigned long long transfer(unsigned long long id, int dir);

  int bitcount(unsigned long long data);
};

void disp(Board board);
void disp_teban(Board board);

inline bool Board::operator==(const Board &rhs) const {
  const Board& lhs = *this;
  return (this->board_black == rhs.board_black) &&
          (this->board_white == rhs.board_white) &&
          (this->turn == rhs.turn);
}

inline bool Board::operator!=(const Board &rhs) const {
  return !(this->operator==(rhs));
}