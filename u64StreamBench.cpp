#include <cstdint>
#include <string>
#include <sstream>
#include <cstring>
#include <chrono>
#include <random>
#include <benchmark/benchmark.h>

uint32_t u64ToAsciiStream(uint64_t value, char *buffer)
{
    std::stringstream ss;
    ss << value;
    auto val_str = ss.str();
    auto val_cstr = val_str.c_str();
    std::strcpy(buffer, val_cstr);
    return val_str.length();
}

static void u64StreamBench(benchmark::State &state)
{
    int size = 1 << state.range(0);
    std::vector<unsigned long> data;
    data.reserve(size);

    // rng for the data and indices
    // rng for the data and indices
    std::default_random_engine random_engine (
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::uniform_int_distribution<int> data_dist{0, std::numeric_limits<int>::max()};

    // insert random data into the vector and random indexes to the index array
    for (auto i = 0; i < size; ++i)
        data.push_back(data_dist(random_engine));

    while (state.KeepRunning())
    {
        char str[64];
        for (auto i : data)
        {
            auto len = u64ToAsciiStream(i, str);
            benchmark::DoNotOptimize(len);
        }
    }

    state.SetItemsProcessed(long(state.iterations()) * long(data.size()));
}

BENCHMARK(u64StreamBench)->DenseRange(5, 20)->ReportAggregatesOnly(true);
BENCHMARK_MAIN();
