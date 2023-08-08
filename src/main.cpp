#include "dekunobou.hpp"
#if defined GA
#include "ga.hpp"
#else
#include "play.hpp"
#endif

#if not defined GA
#include "eval_param.hpp"
#endif

// 対局用の評価関数の読み込み
int load_eval(std::string filename, float param[param_size]) {
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
#if defined Console
  load_eval("eval.txt", param);
  // 人との対決用
  play_engine_human(param);
#elif defined GA
  int threads = 0;
  if (argc > 1)
    threads = atoi(argv[1]);
  ga(threads);
#elif defined API
  // APIの仕様
  // 入力は盤面の文字列と手番の文字列をスペース区切りでコマンド入力で入力する
  // 盤面の文字列: 盤の左上から右下にかけて先手番の石を1, 後手番の石を2,
  // 何もおいてないところを0として
  //              改行が入らないようにすること
  // 手番の文字列: 先手番なら0, 後手番なら1を渡す
  // 出力はAIの指し手を8*(行番号)+(列番号)の0以上63以下の整数になる.
  // 行番号, 列番号はそれぞれ0-indexed
  // 出力が負の数または64以上ならエラー

  Board board;

  // 要求通りの入力じゃなかったら負の数を出力
  if (argc != 3) {
    std::cout << "-1\n";
    return -1;
  }

  if (strlen(argv[1]) != 64) {
    std::cout << "-2\n";
    return -2;
  }

  if (strlen(argv[2]) != 1) {
    std::cout << "-3\n";
    return -3;
  }

  if (argv[2][0] != '0' && argv[2][0] != '1') {
    std::cout << "-4\n";
    return -4;
  }

  // 盤面の文字列に規約違反の文字が含まれていないかみる
  for (int i = 0; i < 64; ++i)
    if (argv[1][i] != '0' && argv[1][i] != '1' && argv[1][i] != '2') {
      std::cout << "-5\n";
      return -5;
    }

  // 盤をセット
  board.point[0] = 0;
  board.point[1] = 0;
  board.board_black = 0;
  board.board_white = 0;
  for (int i = 0; i < 64; ++i) {
    if (argv[1][i] == '1') {
      // 黒石
      board.board_black |=
          (unsigned long long)(1ULL << (unsigned long long)(63 - i));
      ++board.point[0];
    } else if (argv[1][i] == '2') {
      // 白石
      board.board_white |=
          (unsigned long long)(1ULL << (unsigned long long)(63 - i));
      ++board.point[1];
    }
  }

  // 手番をセット
  if (argv[2][0] == '0') {
    board.turn = false;
    board.board_player = board.board_black;
    board.board_opponent = board.board_white;
  } else if (argv[2][0] == '1') {
    board.turn = true;
    board.board_player = board.board_white;
    board.board_opponent = board.board_black;
  }

  std::cout << go(board, param) << std::endl;
#endif
}