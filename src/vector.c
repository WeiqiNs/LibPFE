#include "vector.h"

void vector_zp_from_int(zp_vec x, int *int_vec, int length) {
    for (int i = 0; i < length; i++) zp_from_int(x[i], int_vec[i]);
}

void vector_zp_rand(zp_vec x, int length) {
    for (int i = 0; i < length; i++) rand_zp(x[i]);
}

void vector_merge(zp_vec r, zp_vec a, zp_vec b, int size_a, int size_b) {
    for (int i = 0; i < size_a; i++) zp_copy(r[i], a[i]);
    for (int i = 0; i < size_b; i++) zp_copy(r[i + size_a], b[i]);
}

void vector_add(zp_vec r, zp_vec a, zp_vec b, int size) {
    for (int i = 0; i < size; i++) zp_add(r[i], a[i], b[i]);
}

void vector_raise(ep_st base[1], zp_vec x, g_vec gx, int size) {
    for (int i = 0; i < size; i++) multiply(gx[i], base, x[i]);
}

void inner_product(gt r, g_vec a, g_vec b, int size) {
    pc_map_sim(r, a, b, size);
}






