#include "eval.hpp"
#pragma once
int alphabeta(Board board, int param[param_size], int depth, int alpha,
                int beta);
int go(Board board, int param[param_size], const Option &option);