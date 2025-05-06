#ifndef CHESS_ENG_UTIL_H
#define CHESS_ENG_UTIL_H
#include "types.h"
#include "vector.h"
#include "board.h"
unsigned long long int perft(brd *br, int dph, int m_dph);
int find_best_move(brd *br, pos_int *from, pos_int *to, int m_dph, int dph, int alpha, int beta);
void after_chess();
void bf_chess();
#endif // CHESS_ENG_UTIL_H
