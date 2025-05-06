#ifndef TYPES_H
#define TYPES_H
#define BRD_SIZE 64
#pragma once
typedef struct{
    int x,y;
} pos_int;
enum PIECE{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    EMPTY
};
enum piece_c{
     WH,
     BL,
     EM
};

typedef struct{
    int pieces[BRD_SIZE];
    int piece_colors[BRD_SIZE];
    int LB_castle, RB_castle;
    int LW_castle, RW_castle;
    int checks;
    int history[128];
    int mvs;
    enum piece_c mv;
    pos_int en_passant;
} brd;
typedef struct{
    pos_int* dat; int rel_size; int size;
} vector_pos;
typedef struct{
    pos_int from,to;
} mv;
typedef struct{
    pos_int from, to;
    pos_int sec_from, sec_to; // same when capture
    int castl_revoke_rook;
    int castl_revoke_king;
    int sec_color, sec_type;
    int mv; int prom;
    int LB_castle, RB_castle;
    int LW_castle, RW_castle;
    pos_int en_pas;

}mv_info;
#endif // TYPES_H
