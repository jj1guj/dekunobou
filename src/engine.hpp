#include "eval.hpp"

int nodes;
bool turn_p;

float alphabeta(Board board,float param[param_size],int depth,float alpha,float beta);
int go(Board board,float param[param_size]);