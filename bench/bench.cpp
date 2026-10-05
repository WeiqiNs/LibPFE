#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include "schemes.hpp"

namespace{
    struct Settings{
        int runs;
        std::size_t length;
        std::int64_t bound;
    };

    struct Sample{
        IPFE::IntVec x;
        IPFE::IntVec y;
        QFE::IntMat f;
        std::int64_t value = 0;
    };

    std::vector<Sample> samples(const Settings& settings, const int degree){
        std::mt19937_64 engine(settings.length);
        const auto terms = std::pow(static_cast<double>(settings.length), degree - 1);
        const auto largest = static_cast<std::int64_t>(
            std::pow(static_cast<double>(settings.bound) / terms, 1.0 / degree)
        );
        std::uniform_int_distribution<std::int64_t> entry(0, largest);
        const auto random_vector = [&]{
            IPFE::IntVec v(settings.length);
            for (auto& e : v) e = entry(engine);
            return v;
        };
        std::vector<Sample> result(settings.runs);
        for (auto& [x, y, f, value] : result){
            x = random_vector();
            y = random_vector();
            if (degree == 2){
                for (std::size_t i = 0; i < settings.length; ++i) value += x[i] * y[i];
            }
            else{
                for (std::size_t i = 0; i < settings.length; ++i){
                    f.push_back(random_vector());
                    for (std::size_t j = 0; j < settings.length; ++j) value += x[i] * f[i][j] * y[j];
                }
            }
        }
        return result;
    }

    struct InnerProduct{
        static constexpr int degree = 2;
        static auto key(const auto& msk, const Sample& sample){ return keygen(msk, sample.y); }
        static auto encrypt(const auto& msk, const Sample& sample){ return enc(msk, sample.x); }
    };

    struct Quadratic{
        static constexpr int degree = 3;
        static auto key(const auto& keys, const Sample& sample){ return keygen(keys.msk, sample.f); }
        static auto encrypt(const auto& keys, const Sample& sample){ return enc(keys.pk, sample.x, sample.y); }
    };

    template <class F>
    double milliseconds_per_run(const int runs, F&& f){
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < runs; ++i) f(i);
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count() / runs;
    }

    template <class Family, class Scheme>
    void measure(const Settings& settings){
        const auto inputs = samples(settings, Family::degree);

        std::vector<decltype(Scheme::setup(settings.length))> states;
        const auto setup_ms = milliseconds_per_run(settings.runs, [&](int){
            states.push_back(Scheme::setup(settings.length));
        });
        const auto& state = states.back();

        std::vector<decltype(Family::key(state, inputs.front()))> sks;
        const auto keygen_ms = milliseconds_per_run(settings.runs, [&](const int i){
            sks.push_back(Family::key(state, inputs[i]));
        });

        std::vector<decltype(Family::encrypt(state, inputs.front()))> cts;
        const auto enc_ms = milliseconds_per_run(settings.runs, [&](const int i){
            cts.push_back(Family::encrypt(state, inputs[i]));
        });

        const auto decrypt = Scheme::decryptor(state, 0, settings.bound);
        int wrong = 0;
        const auto dec_ms = milliseconds_per_run(settings.runs, [&](const int i){
            wrong += decrypt(sks[i], cts[i]) != inputs[i].value;
        });
        if (wrong != 0) throw std::runtime_error(std::format("{} decrypted {} values wrongly", Scheme::name, wrong));

        std::cout << std::format(
            "| {} | {:.2f} | {:.2f} | {:.2f} | {:.2f} |\n", Scheme::name, setup_ms, keygen_ms, enc_ms, dec_ms
        );
    }

    template <class Family, class... Schemes>
    void measure_each(const Settings& settings){
        (measure<Family, Schemes>(settings), ...);
    }

    void print_header(const std::string_view curve, const std::string_view values, const Settings& settings){
        std::cout << std::format(
            "\n#### {}, n = {}, {} in [0, {}], mean of {} runs in ms\n\n"
            "| Scheme | Setup | KeyGen | Enc | Dec |\n| --- | ---: | ---: | ---: | ---: |\n",
            curve, settings.length, values, settings.bound, settings.runs
        );
    }

    template <class C>
    void initialize_curve(){
        (void)rbp::pair(rbp::G1<C>::generator(), rbp::G2<C>::generator());
    }

    template <class C>
    void benchmark(const Settings& settings){
        initialize_curve<C>();
        print_header(C::name, "inner products", settings);
        measure_each<InnerProduct, Bjk<C>, Tao<C>, Kim<C>, Lin<C>, Kks<C>, Opt<C>>(settings);
        print_header(C::name, "quadratic forms", settings);
        measure_each<Quadratic, Bcfg<C>, Sgp<C>>(settings);

        const auto table_ms = milliseconds_per_run(1, [&](int){
            (void)rbp::DlogTable<C>(rbp::Gt<C>::generator(), 0, settings.bound);
        });
        std::cout << std::format(
            "\nOne discrete-log table for [0, {}] takes {:.2f} ms to build.\n", settings.bound, table_ms
        );
    }

    template <class... Curves>
    void benchmark_every_curve(const Settings& settings){
        (benchmark<Curves>(settings), ...);
    }
}

int main(const int argc, char** argv){
    const int runs = argc > 1 ? std::stoi(argv[1]) : 10;
    std::vector<std::size_t> lengths;
    for (int i = 2; i < argc; ++i) lengths.push_back(std::stoul(argv[i]));
    if (lengths.empty()) lengths = {10, 100};

    for (const auto length : lengths){
        benchmark_every_curve<rbp::BLS12_381, rbp::BN254, rbp::SS1536>({runs, length, 10000});
    }
}
