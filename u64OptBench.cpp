#include <vector>
#include <chrono>
#include <random>
#include <benchmark/benchmark.h>

uint32_t digits10_O1(uint64_t num) __attribute__((__always_inline__));

uint32_t digits10_O1(uint64_t num)
{
    uint32_t result = 1;
    for (;;)
    {
        if (num < 10) return result;
        if (num < 100) return result + 1;
        if (num < 1000) return result + 2;
        if (num < 10000) return result + 3;
        num /= 10000U;
        result += 4;
    }

    return result;
}

uint32_t u64ToAsciiOpt(uint64_t num, char *const buffer)
{
    auto const result = digits10_O1(num);
    uint32_t pos = result - 1;

    while (num >= 10)
    {
        auto const q = num / 10;
        auto const r = static_cast<uint32_t>(num % 10);
        buffer[pos--] = '0' + r;
        num = q;
    }

    *buffer = static_cast<uint32_t>(num) + '0';
    return result;
}


static void u46OptBench(benchmark::State &state)
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
            auto len = u64ToAsciiOpt(i, str);
            benchmark::DoNotOptimize(len);
        }   
    }

    state.SetItemsProcessed(long(state.iterations()) * long(data.size()));
}

BENCHMARK(u46OptBench)->DenseRange(5, 20)->ReportAggregatesOnly(true);
BENCHMARK_MAIN();
