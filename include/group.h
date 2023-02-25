#ifndef PPANN_GROUP_H
#define PPANN_GROUP_H

#include "field.h"

typedef g1_t g;
typedef gt_t gt;

void generator(g x);

void negate(g nx, g x);

void multiply(g r, g x, zp y);

void exponentiation(gt r, gt x, zp y);

void map(g a, g b, gt r);

#endif //PPANN_GROUP_H
