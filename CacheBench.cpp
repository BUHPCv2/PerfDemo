#include <vector>
#include <benchmark/benchmark.h>
#include <limits>
#include <random>
#include <chrono>
#include <iostream>

static void cacheBench(benchmark::State &state)
{
    int bytes = 1 << state.range(0);
    int size = ( bytes / sizeof(int) ) / 2;

    std::vector<int> data;
    data.reserve(size);
    std::vector<int> indices;
    indices.reserve(size);

    // rng for the data and indices
    std::default_random_engine random_engine (
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::uniform_int_distribution<int> data_dist {
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    };
    std::uniform_int_distribution<int> idx_dist {0, size};

    // insert random data into the vector and random indexes to the index array
    for (auto i = 0; i < size; ++i)
        data.push_back(data_dist(random_engine));

    // now pick random indices and insert into index array
    for (auto i = 0; i < size; ++i)
        indices.push_back(idx_dist(random_engine));

    // thrash all cache!!
    while (state.KeepRunning())
    {
        auto sum = 0LL;
        for (auto i : indices)
            sum += data[i];

        benchmark::DoNotOptimize(sum);
        __asm__ volatile
            ("" : : "g"(sum) : "memory");
    }

    state.SetBytesProcessed(long(state.iterations()) * long(bytes));
    state.SetLabel(std::to_string(bytes / 1024) + "kb");
}

BENCHMARK(cacheBench)->DenseRange(10, 20)->ReportAggregatesOnly(true);

BENCHMARK_MAIN();
