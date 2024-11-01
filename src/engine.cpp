#include "engine.hpp"

#include <chrono>
#include <unordered_map>

#include "board.hpp"
#include "legalmovelist.hpp"

long long nodes;
long long nodes_total = 0;
bool turn_p;
float cell_weight[64] = {
    30,  -12, 0,  -1, -1, 0,  -12, 30,  -12, -15, -3, -3, -3, -3, -15, -12,
    0,   -3,  0,  -1, -1, 0,  -3,  0,   -1,  -3,  -1, -1, -1, -1, -3,  -1,
    -1,  -3,  -1, -1, -1, -1, -3,  -1,  0,   -3,  0,  -1, -1, 0,  -3,  0,
    -12, -15, -3, -3, -3, -3, -15, -12, 30,  -12, 0,  -1, -1, 0,  -12, 30};

void move_ordering(LegalMoveList &moves, Board board, float param[param_size]) {
  // 各要素を{評価値, 候補手}としておき,
  // 評価値と候補手を紐づけることで二重ループを回避する
  std::vector<std::pair<float, int>> evals(moves.size());
  // Board board_ref;

  // 1手読みの評価値を算出
  for (int i = 0; i < moves.size(); ++i) {
    // board_ref=board;
    // board_ref.push(moves[i]);
    // evals[i]={-eval(board_ref,param),moves[i]};
    evals[i] = {cell_weight[moves[i]], moves[i]};
  }

  // 評価値の降順にソート
  std::sort(evals.begin(), evals.end(), std::greater<std::pair<float, int>>());

  for (int i = 0; i < moves.size(); ++i) moves[i] = evals[i].second;
}

// αβ法による先読み
// α: 評価値の最小値
// β: 評価値の最大値
float alphabeta(Board board, float param[param_size], int depth, float alpha,
                float beta) {
  // 候補手の展開
  LegalMoveList moves(board);
  move_ordering(moves, board, param);
  if (moves.size() == 0) {
    // 手番を変えて展開
    board.push(-1);
    LegalMoveList moves2(board);
    move_ordering(moves2, board, param);
    // 終局
    if (moves2.size() == 0) {
      ++nodes;
      return board.point[turn_p] - board.point[!turn_p];
    }
    return alphabeta(board, param, depth - 1, alpha, beta);
    moves = moves2;
  }

  float val;
  if (board.turn == turn_p)
    val = -inf;  // エンジン側が手番のときは評価値の最大値を求める
  else
    val = inf;  // 相手が手番のときは評価値の最小値を求める
  // 末端ノードのとき
  Board board_ref;
  if (depth <= 0) {
    for (int i = 0; i < moves.size(); ++i) {
      ++nodes;
      board_ref = board;
      board_ref.push(moves[i]);
      if (board.turn == turn_p) {
        val = std::max(val, eval(board_ref, param));
        if (val >= beta) break;
        alpha = std::max(alpha, val);
      } else {
        val = std::min(val, eval(board_ref, param));
        if (val <= alpha) break;
        beta = std::min(beta, val);
      }
    }
    return val;
  }

  for (int i = 0; i < moves.size(); ++i) {
    // 1手打つ
    board_ref = board;
    board_ref.push(moves[i]);
    if (board.turn == turn_p) {
      val = std::max(val, alphabeta(board_ref, param, depth - 1, alpha, beta));
      if (val >= beta) break;
      alpha = std::max(alpha, val);
    } else {
      val = std::min(val, alphabeta(board_ref, param, depth - 1, alpha, beta));
      if (val <= alpha) break;
      beta = std::min(beta, val);
    }
  }
  return val;
}

// オセロAIの教科書をもとに実装
// https://note.com/nyanyan_cubetech/n/n210cf134b8b1?magazine_key=m54104c8d2f12
float nega_alpha(Board &board,
                 std::unordered_map<Board, float, Board::Hash> &transpose_table,
                 float param[param_size], int depth, bool passed, float alpha,
                 float beta) {
  // 末端ノードでは評価関数を呼ぶ
  if (depth == 0) {
    ++nodes;
    return eval(board, param);
  }

  // 置換表にヒットしたら置換表に格納されているminimax値を返す
  if (transpose_table.find(board) != transpose_table.end()) {
    return transpose_table[board];
  }

  LegalMoveList moves(board);
  move_ordering(moves, board, param);

  float max_score = -inf;
  for (int i = 0; i < moves.size(); ++i) {
    Board board_ref = board;
    board_ref.push(moves[i]);
    float g = -nega_alpha(board_ref, transpose_table, param, depth - 1, false,
                          -beta, -alpha);
    if (g >= beta) return g;
    alpha = std::max(alpha, g);
    max_score = std::max(max_score, g);
  }

  if (max_score == -inf) {
    if (passed) {
      ++nodes;
      return board.point[board.turn] - board.point[!board.turn];
    }
    board.push(-1);  // 手番を変えて探索する
    return -nega_alpha(board, transpose_table, param, depth, true, -beta,
                       -alpha);
  }

  transpose_table[board] = max_score;
  return max_score;
}

int go(Board board, float param[param_size], const Option &option) {
  std::unordered_map<Board, float, Board::Hash> transpose_table;
  std::chrono::system_clock::time_point start, end;
  transpose_table.clear();
  turn_p = board.turn;

  float val = -inf;
  LegalMoveList moves(board);
  // 1手だけのときはその手を返す
  if (moves.size() == 1) return moves[0];

  turn_p = board.turn;  // エンジン側の手番を取得

  int BestMoves[64];
  int bestmoves_num;
  std::random_device rnd_select;

  bestmoves_num = 0;
  float eval_ref;

  // 現在の評価値を算出
  Board board_ref;

  // 探索の優先順位付け
  float evals[64];
  int priority[64];
  bool selected[64];
  std::vector<float> evals_sort(moves.size());

  if (option.debug) {
    start = std::chrono::system_clock::now();
  }

  if (option.mode != Mode::ga) {
    // 5手読みの評価値を算出
    for (int i = 0; i < moves.size(); ++i) {
      board_ref = board;
      board_ref.push(moves[i]);
      evals[i] = alphabeta(board_ref, param, 4, val, inf);
      evals_sort[i] = evals[i];
      val = std::max(evals[i], val);
      selected[i] = false;
    }

    // 評価値の降順にソート
    std::sort(evals_sort.begin(), evals_sort.end(), std::greater<float>());

    for (int i = 0; i < moves.size(); ++i) {
      for (int j = 0; j < moves.size(); ++j) {
        if (!selected[j] && evals_sort[i] == evals[j]) {
          priority[i] = j;
          selected[j] = true;
          break;
        }
      }
    }

    if (option.debug) {
      std::cout << "priority: ";
      for (int i = 0; i < moves.size(); ++i)
        std::cout << priority[i] + 1 << " ";
      std::cout << std::endl;
      if (board.point[0] + board.point[1] >= 49)
        std::cout << "depth: " << 63 - board.point[0] - board.point[1]
                  << std::endl;
    }
  } else {
    for (int i = 0; i < moves.size(); ++i) {
      priority[i] = i;
    }
  }

  val = -inf;
  float alpha = -inf, beta = inf;
  for (int i = 0; i < moves.size(); i++) {
    // 先読みしてみる
    // 1手読みしたいなら深さを0に指定する
    board_ref = board;
    board_ref.push(moves[priority[i]]);

    if (option.mode != Mode::ga) {
      // 終盤20手で完全読み
      nodes = 0;
      if (board.point[0] + board.point[1] >=
          60 - option.option_web.perfect_search_depth)
        eval_ref = -nega_alpha(board_ref, transpose_table, param, 60, false,
                               -beta, -alpha);
      else
        eval_ref =
            -nega_alpha(board_ref, transpose_table, param,
                        option.option_web.depth - 1, false, -beta, -alpha);
      if (alpha < eval_ref) alpha = eval_ref;
      if (option.debug) {
        nodes_total += nodes;
        std::cout << priority[i] + 1 << "(" << moves[priority[i]] << ")" << ": "
                  << eval_ref << " " << nodes / 1000 << "k" << std::endl;
      }
    } else {
      eval_ref =
          -nega_alpha(board_ref, transpose_table, param, 0, false, -inf, inf);
    }

    if (eval_ref > val) {
      bestmoves_num = 0;
      BestMoves[bestmoves_num] = moves[priority[i]];
      ++bestmoves_num;
      val = eval_ref;
    } else if (eval_ref == val &&
               board.point[0] + board.point[1] <
                   60 - option.option_web.perfect_search_depth) {
      BestMoves[bestmoves_num] = moves[priority[i]];
      ++bestmoves_num;
    }
  }

  if (option.debug) {
    // for debug
    end = std::chrono::system_clock::now();
    double elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "eval: " << val << std::endl;
    std::cout << "nodes: " << nodes_total / 1000 << "k" << std::endl;
    std::cout << "time: " << elapsed << "[msec]" << std::endl;
  }

  int tmp = rnd_select() % bestmoves_num;
  transpose_table.clear();
  transpose_table = std::unordered_map<Board, float, Board::Hash>();
  return BestMoves[tmp];
}