#ifndef PPANN_VECTOR_H
#define PPANN_VECTOR_H

#include "field.h"
#include "group.h"

typedef ZP *zp_vec;
typedef g *g_vec;

zp_vec vector_zp_from_int(const int *int_vec, int size, bn_t modular);

zp_vec vector_zp_rand(int size, bn_t modular);

zp_vec vector_merge(zp_vec a, zp_vec b, int size_a, int size_b);

zp_vec vector_add(zp_vec a, zp_vec b, int size);

g_vec vector_raise(g base, zp_vec x, int size);

void inner_product(gt r, g_vec a, g_vec b, int size);

#endif //PPANN_VECTOR_H