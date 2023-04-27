#include "helper.h"

int test_read_fvecs() {
    // Set dimensions holders and get the data.
    size_t d, n;
    float *xd = fvecs_read("sift_query.fvecs", &d, &n);

    // Check for whether the data is correct.
    if (d != 128) return 0;
    if (n != 10000) return 0;
    if (xd[1] != 3) return 0;
    if (xd[1279872] != 23) return 0;

    // If everything passes, return 1.
    return 1;
}

int test_read_ivecs() {
    // Set dimensions holders and get the data.
    size_t d, n;
    int *gt = ivecs_read("sift_groundtruth.ivecs", &d, &n);

    // Check for whether the data is correct.
    if (d != 100) return 0;
    if (n != 10000) return 0;
    if (gt[1] != 934876) return 0;
    if (gt[999900] != 874343) return 0;

    // If everything passes, return 1.
    return 1;
}

int test_float_to_int() {
    // Set dimensions holders and get the data.
    size_t d, n;
    float *xd = fvecs_read("sift_query.fvecs", &d, &n);

    // Conversion and test.
    int *data = float_to_int(xd, d * n);

    // Check for whether the data is correct.
    if (data[1] != 3) return 0;
    if (data[1279872] != 23) return 0;

    // If everything passes, return 1.
    return 1;
}

int test_encrypt() {
    // Set dimensions holders and get the data.
    size_t d, n;
    float *xd = fvecs_read("sift_query.fvecs", &d, &n);

    // Conversion.
    int *data = float_to_int(xd, d * n);

    // Cast d to integer.
    int d_int = static_cast<int>(d);

    // Encrypt the first two vectors.
    Key key = setup(d_int);
    Item *encrypted_data = encrypt_data(data, key, d, 2);

    // Get inner product of the first two vectors.
    int result = eval(key, encrypted_data[0].value, encrypted_data[1].value, d_int, 200000);

    // Check for whether the data is correct.
    if (result != 184094) return 0;

    return 1;
}

int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_read_fvecs() != 1) return 1;
    if (test_read_ivecs() != 1) return 1;
    if (test_float_to_int() != 1) return 1;
    if (test_encrypt() != 1) return 1;
    return 0;
}