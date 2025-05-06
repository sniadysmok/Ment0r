#ifndef BOARD_H
#define BOARD_H
#pragma once
#include "types.h"
#include "vector.h"
#include <stdio.h>
//contains functions relating move generation and board management
void init_board(brd * br, int pic[BRD_SIZE], int col[BRD_SIZE]);
pos_int add_pos(pos_int p1, pos_int p2);
void read_fen_to_board(brd *br, const char *c);
inline static int pos_to_index(pos_int p){
    return (p.x + p.y*8);
}
inline static pos_int index_to_pos(int i){
    int x,y;
    x = i%8;
    y = i/8;
    return (pos_int){x,y};
}
inline static int is_in_bounds(pos_int p){
    if(p.x > -1 && p.y > -1 && p.x < 8 && p.y < 8){
        return 1;
    }
    return 0;
}

int is_tile_under_attack(int atk_color,int my_color, brd *br, pos_int tile);
void generate_moves(brd *br, vector_pos * from, vector_pos *to);
void paint_possible_moves(pos_int pos, const vector_pos from, const vector_pos to, int col[BRD_SIZE]);
void make_move(pos_int from, pos_int to, brd *br, mv_info *info);
void undo_move(mv_info info, brd *br);
#endif // BOARD_H
