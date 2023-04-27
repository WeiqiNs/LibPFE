#ifndef PPANN_IPRE_H
#define PPANN_IPRE_H

#include "vector.h"
#include "matrix.h"

const int B_SIZE = 6;

struct Key {
    zp_mat A;
    zp_mat B;
    zp_mat Bi;
    g base;
    gt t_base;
    bn_t modular;
};

struct Ct {
    g_vec ctx;
    g_vec ctk;
    g_vec ctc;
};

Key setup(int size);

Ct enc(Key key, const int *message, int size);

int eval(Key key, Ct x, Ct y, int size, int bound);

#endif //PPANN_IPRE_H