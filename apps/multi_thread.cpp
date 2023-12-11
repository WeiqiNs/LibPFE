#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include "asym_ipfe.hpp"

int main() {
    // Seed the random number generator with a random device
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range for the random values
    int low = 1, up = 300;
    // Create a distribution for random integers in the specified range
    std::uniform_int_distribution<int> distribution(low, up);

    // Initialize the scheme.
    asym::ipfe::Pp ipre_pp = asym::ipfe::ppgen(true, 10, 150);
    asym::ipfe::Sk ipre_sk = asym::ipfe::setup(ipre_pp);

    // Generate some dummy data.
    int vector_amount = 5000;
    int vectors[5000][128];
    for (auto &vector: vectors) for (auto &i: vector) i = distribution(gen);

    // Perform encryption without threading.
    auto start = std::chrono::high_resolution_clock::now();

    for (auto &vector: vectors) asym::ipfe::enc(ipre_pp, ipre_sk, vector);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Not threaded takes: " << duration.count() << " seconds" << std::endl;

    // Declare threads.
    std::vector<std::thread> threads;
    // Get the number of threads.
    auto num_thread = std::thread::hardware_concurrency();
    // Get workload per thread.
    auto work_per_thread = vector_amount / num_thread;

    // Perform encryption with threading.
    start = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < num_thread; ++i) {
        auto start_id = i * work_per_thread;
        auto end_id = (i == num_thread - 1) ? vector_amount : start_id + work_per_thread;

        threads.emplace_back([start_id, end_id, ipre_pp, ipre_sk, vectors]() {
            for (auto i = start_id; i < end_id; i++) {
                asym::ipfe::enc(ipre_pp, ipre_sk, vectors[i]);
            }
        });
    }

    // Join threads to the main thread
    for (int i = 0; i < num_thread; ++i) {
        threads[i].join();
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Threaded takes: " << duration.count() << " seconds" << std::endl;

    return 0;
}