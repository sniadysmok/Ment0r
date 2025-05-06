#ifndef EVAL_H
#define EVAL_H
#include "types.h"
int eval(brd *br);
void init_tables();
void init_table_h();
unsigned long long int zbr_hash(brd *br);
#endif // EVAL_H
