#include <stdio.h>
#include <omp.h>
#include <chrono>
#include <valarray>

// #include "Algorithm.h"
#include "Benchmark.h"

#define LAB2

#ifdef LAB1
#include "lab1/MultiplyMatrixLinear.h"
#include "lab1/MultiplyMatrixParallel.h"
#endif
#ifdef LAB2
#include "lab2/FloydWarshallLinear.h"
#include "lab2/FloydWarshallParallel.h"
#endif


using namespace std;
using namespace chrono;


auto test_repeat = 10;

void bench_parallel(valarray<int> amounts, Algorithm* alg)
{
    valarray<int> thread_count = {2, 4, 8, 16, 32};
    omp_set_dynamic(0);
    for (size_t ti = 0; ti < thread_count.size(); ti++)
    {
        omp_set_num_threads(thread_count[ti]);
        for (size_t i = 0; i < amounts.size(); i++)
        {
            int n = amounts[i];
            Benchmark::bench(thread_count[ti], n, test_repeat, alg);
        }
    }
}

void bench_linear(valarray<int> amounts, Algorithm* alg)
{
    omp_set_dynamic(0);
    for (size_t i = 0; i < amounts.size(); i++)
    {
        int n = amounts[i];
        Benchmark::bench(1, n, test_repeat, alg);
    }
    return;
}

int main()
{
    srand(16);
    #ifdef _OPENMP
    printf("[*]Compiled with OpenMP\n");
    printf("[*]Maximum number of threads: %d\n", omp_get_max_threads());
    #endif
    #ifdef LAB1
    valarray<int> test_amount = {500, 600, 700, 800, 900, 1000, 5000, 10000};
    bench_linear(test_amount, new MultiplyMatrixLinear());
    bench_parallel(test_amount, new MultiplyMatrixParallel());
    #endif
    #ifdef LAB2
    valarray<int> test_amount = {100, 200, 300, 400, 500};
    bench_linear(test_amount, new FloydWarshallLinear());
    bench_parallel(test_amount, new FloydWarshallParallel());
    #endif
}
