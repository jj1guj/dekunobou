#include "dekunobou.hpp"
#include "eval_param.hpp"
#include "ga.hpp"
#include "hash.hpp"
#include "play.hpp"

// ZobristHash zobrist;

// 対局用の評価関数の読み込み
int load_eval(std::string filename, int param[param_size]) {
  std::ifstream inputs(filename);
  std::string s;
  int i = 0;
  if (inputs.fail()) {
    std::cout << "Failed to openfile\n";
    return -1;
  }
  while (getline(inputs, s)) {
    param[i] = std::stof(s);
    std::cout << param[i] << std::endl;
    ++i;
  }
  return 0;
}

int main(int argc, char **argv) {
  // コマンドライン引数をパースしてオプションをセットする
  Option option;
  int status = set_option(argc, argv, option);

  // オプションのセットに失敗したらその時点で終了する
  if (status != 0) return status;

  if (option.mode == Mode::ga) {
    ga(option);
  } else if (option.mode == Mode::web) {
    // 評価関数ファイルが指定されていたら読み込む
    if (!option.option_web.eval.empty()) {
      status = load_eval(option.option_web.eval, param);
      // 読み込みに失敗したらその時点で終了する
      if (status != 0) return status;
    }

    Board board;

    // 盤をセット
    board.point[0] = 0;
    board.point[1] = 0;
    board.board_black = 0;
    board.board_white = 0;
    for (int i = 0; i < 64; ++i) {
      if (option.option_web.board[i] == '1') {
        // 黒石
        board.board_black |=
            (unsigned long long)(1ULL << (unsigned long long)(63 - i));
        ++board.point[0];
      } else if (option.option_web.board[i] == '2') {
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
    std::cout << go(board, param, option) << std::endl;
  }

  return 0;
}