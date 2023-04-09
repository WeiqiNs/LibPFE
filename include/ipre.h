#ifndef PPANN_IPRE_H
#define PPANN_IPRE_H

#include "vector.h"
#include "matrix.h"

const int B_SIZE = 6;

struct key {
    zp_mat A;
    zp_mat B;
    zp_mat Bi;
    g base;
    gt t_base;
    bn_t modular;
};

struct ct {
    g_vec ctx;
    g_vec ctk;
    g_vec ctc;
};

key setup(int size);

ct enc(key key, const int *message, int size);

int eval(key key, ct x, ct y, int size, int bound);

#endif //PPANN_IPRE_H