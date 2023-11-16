#pragma once

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include "hnsw.h"
#include "sym_ipre.hpp"

float *fvecs_read(const char *file_path, size_t *d_out, size_t *n_out);

int *ivecs_read(const char *file_path, size_t *d_out, size_t *n_out);

int *float_to_int(const float *data, size_t size);

Item *encrypt_data(const int *data, Key key, size_t d, size_t n);