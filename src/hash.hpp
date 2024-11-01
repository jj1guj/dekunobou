#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

#include "board.hpp"

struct Board::Hash {
  typedef std::size_t result_type;
  std::size_t operator()(const Board& board) const;
};

inline std::size_t Board::Hash::operator()(const Board& board) const {
  std::string bytes(reinterpret_cast<const char*>(&board), sizeof(Board));
  return std::hash<std::string>()(bytes);
}