#include "ipre.h"
#include "hnsw.h"
#include "helper.h"

int test_constructor() {
    // Get the key.
    Key key = setup(10);

    // Get the HNSW object.
    HNSWGraph index(10, 30, 10, 40, 4, key, 10, 100);

    // Check for whether the data is correct.
    if (index.NN != 10) return 0;

    // If everything passes, return 1.
    return 1;
}

int test_insert() {
    // Set dimensions holders and get the data.
    size_t d, n;
    float *xd = fvecs_read("sift_query.fvecs", &d, &n);

    // Conversion.
    int *data = float_to_int(xd, d * n);

    // Cast d to integer.
    int d_int = static_cast<int>(d);

    // Encrypt the first three vectors.
    Key key = setup(d_int);
    Item *encrypted_data = encrypt_data(data, key, d, 3);

    // Get the HNSW object.
    HNSWGraph index(10, 30, 10, 40, 4, key, 10, 100);

    // Do the insert.
    for (int i = 0; i < 3; i++) index.insert(encrypted_data[i]);

    // Check for whether the data is correct.
    if (index.numItem != 3) return 0;

    return 1;
}

int test_search() {
    // Set dimensions holders and get the data.
    size_t d, n;
    float *xd = fvecs_read("sift_query.fvecs", &d, &n);

    // Conversion.
    int *data = float_to_int(xd, d * n);

    // Cast d to integer.
    int d_int = static_cast<int>(d);

    // Encrypt the first three vectors.
    Key key = setup(d_int);
    Item *encrypted_data = encrypt_data(data, key, d, 6);

    // Get the HNSW object.
    HNSWGraph index(10, 30, 10, 40, 4, key, 10, 100);

    // Do the insert.
    for (int i = 0; i < 5; i++) index.insert(encrypted_data[i]);

    // Check for whether the data is correct.
    vector<int> result = index.search(encrypted_data[5], 3);

    if (result.size() != 3) return 0;

    return 1;
}


int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_constructor() != 1) return 1;
    if (test_insert() != 1) return 1;
    if (test_search() != 1) return 1;

    return 0;
}