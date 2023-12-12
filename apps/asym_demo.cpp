#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <iostream>
#include "asym_ipfe.hpp"


int main() {
    // Seed the random number generator with a random device and create a distribution.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 300);

    // Set dimensions holders and get the data.
    int data_dim = 128, data_size = 10000;

    // Generate some dummy data.
    std::vector<std::vector<int>> vectors(data_size, std::vector<int>(data_dim));
    for (auto &vector: vectors) for (auto &data: vector) data = distribution(gen);

    // Initialize the scheme.
    asym::ipfe::Pp ipre_pp = asym::ipfe::ppgen(true, data_dim, 1);
    asym::ipfe::Sk ipre_sk = asym::ipfe::setup(ipre_pp);

    // Declare threads.
    std::vector<std::thread> threads;
    // Get the number of threads.
    auto num_thread = std::thread::hardware_concurrency();
    // Get workload per thread.
    auto work_per_thread = data_size / num_thread;

    // Perform encryption with threading.
    auto start = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < num_thread; i++) {
        // Set start and end ID.
        auto start_id = i * work_per_thread;
        auto end_id = (i == num_thread - 1) ? data_size : start_id + work_per_thread;

        // Push work to threads.
        threads.emplace_back([start_id, end_id, ipre_pp, ipre_sk, vectors]() {
            for (auto j = start_id; j < end_id; j++) asym::ipfe::enc(ipre_pp, ipre_sk, vectors[j].data());
        });
    }

    // Join threads to the main thread
    for (int i = 0; i < num_thread; ++i) threads[i].join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "The threaded process takes " << duration.count() << " seconds." << std::endl;

    return 0;
}