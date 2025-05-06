#ifndef VECTOR_H
#define VECTOR_H
#pragma once
#include "types.h"
void init(  vector_pos * const vect);
void push_back(pos_int elem, vector_pos * const vect);
void clear(vector_pos * const vect);
int contains(pos_int elem, pos_int elem2, const vector_pos vect, const vector_pos vect2);
#endif
