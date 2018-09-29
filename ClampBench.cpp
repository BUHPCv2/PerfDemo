#include <vector>
#include <bencmark/benchmark.h>
#include <limits>
#include <random>
#include <chrono>

static void clampBench(benchmark::State &state)
{
    auto size = 1 << state.range(0);
    std::vector<int> data;
    data.reserve(size);

    // rng for the data and indices
    std::default_random_engine random_engine (std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> data_dist {
        std::numeric_limits<int>::min,
        std::numeric_limits<int>::max
    };

    while (state.KeepRunning())
    {
        // switch off optimizations so we can see what is acutally happening
#pragma clang loop vectorize(disable) interleave(disable) unroll(disable)
        for (int &i : data)
            i = i > 255 ? 255 : i;
        benchmark::DoNotOptimize(data);

        state.SetItemsProcessed(long(state.iterations()) * long(data.size()));
    }

    state.SetBytesProcessed(long(state.iterations() * long(bytes));
    state.SetLabel(std::to_string(bytes / 1024) + "kb");
}

BENCHMARK(clampBench)->DenseRange(10, 20)->ReportAggregateOnly(true);

BENCHMARK_MAIN();
