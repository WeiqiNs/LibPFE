#include "helper.h"

float *fvecs_read(const char *file_path, size_t *d_out, size_t *n_out) {
    // Open up the file.
    FILE *file = fopen(file_path, "r");

    // Get the dimension.
    int d;
    fread(&d, 1, sizeof(int), file);

    // Get the number of vectors.
    fseek(file, 0, SEEK_SET);
    struct stat st{};
    fstat(fileno(file), &st);

    // Compute number of vectors.
    size_t sz = st.st_size;
    size_t n = sz / ((d + 1) * 4);

    // Update parameters.
    *d_out = d;
    *n_out = n;

    // Save the data.
    auto *x = new float[n * (d + 1)];
    fread(x, sizeof(float), n * (d + 1), file);

    // shift array to remove row headers
    for (size_t i = 0; i < n; i++) memmove(x + i * d, x + 1 + i * (d + 1), d * sizeof(*x));

    // Close file.
    fclose(file);

    return x;
}

int *ivecs_read(const char *file_path, size_t *d_out, size_t *n_out) {
    // Cast the float results to integers.
    return (int *) fvecs_read(file_path, d_out, n_out);
}

int *float_to_int(const float *data, size_t size) {
    // Get a new list for integer data.
    auto int_data = new int[size];

    // Cast float to integers.
    for (int i = 0; i < size; i++) int_data[i] = static_cast<int>(data[i]);

    // Return pointer of the list.
    return int_data;
}

Item *encrypt_data(const int *data, Key key, size_t d, size_t n) {
    // Get a new list for encrypted data.
    auto *encrypted_data = new Item[n];

    // Encrypt each vector.
    for (int i = 0; i < n; i++) encrypted_data[i].value = enc(key, &data[i * d], static_cast<int>(d));

    // Return pointer of the list.
    return encrypted_data;
}