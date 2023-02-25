#ifndef PPANN_VECTOR_H
#define PPANN_VECTOR_H

#include "field.h"
#include "group.h"

typedef zp *zp_vec;
typedef g *g_vec;

void vector_zp_from_int(zp_vec x, int *int_vec, int length);

void vector_zp_rand(zp_vec x, int length);

void vector_merge(zp_vec r, zp_vec a, zp_vec b, int size_a, int size_b);

void vector_add(zp_vec r, zp_vec a, zp_vec b, int size);

void vector_raise(g base, zp_vec x, g_vec gx, int size);

void inner_product(gt r, g_vec a, g_vec b, int size);

#endif //PPANN_VECTOR_H