#ifndef PPANN_GROUP_H
#define PPANN_GROUP_H

#include "field.h"

typedef g1_t g;
typedef gt_t gt;

void gen(g x);

void g_mul(g r, g x, zp y);

void gt_raise(gt r, gt x, zp y);

void bp_map(g a, g b, gt r);

#endif //PPANN_GROUP_H