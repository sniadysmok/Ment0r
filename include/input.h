#ifndef INPUT_H
#define INPUT_H
#pragma once
#include "types.h"
#include "board.h"
#include <unistd.h>
#include "vector.h"
#include "raylib.h"
#include <stdio.h>
typedef struct{
    pos_int selct; // (-1,-1) when none
}io_manager;
void init_io(io_manager *mg);
void slct(io_manager *mg, int flip);
void dslct(io_manager *mg);
void reset_paint(int clr[BRD_SIZE]);
void paint(int x, int y, int clr_mask[BRD_SIZE], int val);
void move(brd *br, pos_int from, pos_int to, int col_mask[BRD_SIZE]);
void mouseio(int is_flipped, io_manager *mf, io_manager *mt, int col_mask[64], brd *br, int *m_c);
#endif
