#include "hash.hpp"
#include <bitset>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>

void ZobristHash::init() {
  unsigned long long seed =
      std::chrono::steady_clock::now().time_since_epoch().count();
  std::mt19937_64 r_mt64bit(seed);

  // 下位1ビットを手番情報にするため常に偶数になるようにする
  // 割り当てる乱数はすべて10本以上のビットが立っているようにする
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 65536; ++j) {
      hash_player[i][j] = r_mt64bit() & ~UINT64_C(1);
      while (std::bitset<64>(hash_player[i][j]).count() < 10) {
        hash_player[i][j] = r_mt64bit() & ~UINT64_C(1);
      }

      hash_opponent[i][j] = r_mt64bit() & ~UINT64_C(1);
      while (std::bitset<64>(hash_opponent[i][j]).count() < 10) {
        hash_opponent[i][j] = r_mt64bit() & ~UINT64_C(1);
      }
    }
  }
}

unsigned long long ZobristHash::hash(Board board) {
  uint16_t *board_player = reinterpret_cast<uint16_t *>(&board.board_player);
  uint16_t *board_opponent =
      reinterpret_cast<uint16_t *>(&board.board_opponent);

  return hash_player[0][board_player[0]] ^ hash_player[1][board_player[1]] ^
         hash_player[2][board_player[2]] ^ hash_player[3][board_player[3]] ^
         hash_opponent[0][board_opponent[0]] ^
         hash_opponent[1][board_opponent[1]] ^
         hash_opponent[2][board_opponent[2]] ^
         hash_opponent[3][board_opponent[3]] ^ board.turn;
}

unsigned long long ZobristHash::hash_pass(unsigned long long hash) {
  // パスの際は手番情報を反転すれば良いので1とのXORを取る
  return hash ^ UINT64_C(1);
};