#pragma once

#include "board.hpp"
#include <cstdint>
class ZobristHash {
public:
  ZobristHash() { init(); }
  unsigned long long hash(Board board);
  unsigned long long hash_pass(unsigned long long hash);

private:
  void init();
  unsigned long long hash_player[4][65536];
  unsigned long long hash_opponent[4][65536];
};