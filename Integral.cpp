
// ==================================================
// Parallel Definite Integral Calculation using OpenMP
// Course: Parallel Computing
// ==================================================

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <omp.h>
#include <vector>
#include <iomanip>

using namespace std;

// High-resolution timer for Windows
double GetTime();

// Function to integrate
double Function(double x) {
    return exp(-x * x) + sin(x);
}

// Serial integration using rectangle (left point) method
double SerialIntegralValue(double a, double b, long long int N) {
    double h = (b - a) / N;
    double sum = 0;
    for (long long int i = 0; i < N; i++) {
        sum += Function(i * h + a);
    }
    return sum * h;
}

// Parallel integration using OpenMP reduction
double ParallelIntegralValue(double a, double b, long long int N) {
    double h = (b - a) / N;
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (long long int i = 0; i < N; i++) {
        sum += Function(i * h + a);
    }
    return sum * h;
}

// High-resolution timer for Windows
double GetTime() {
    LARGE_INTEGER lpFrequency, lpPerfomanceCount;
    QueryPerformanceFrequency(&lpFrequency);
    QueryPerformanceCounter(&lpPerfomanceCount);
    return (double)lpPerfomanceCount.QuadPart / (double)lpFrequency.QuadPart;
}

int main() {
    double a = 0, b = 10;
    vector<long long int> N_values = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 500000000};
    int threadCounts[] = {2, 4};

    cout << "--- Parallel Integral Calculation ---\n";

    FILE* file = fopen("integral_results.csv", "w");
    if (!file) return 1;
    fprintf(file, "Size,Threads,ExecutionType,Time,Speedup,Efficiency,Result\n");

    for (auto N : N_values) {
        printf("\nProcessing N = %lld\n", N);

        // Serial calculation
        double startS = GetTime();
        double resSerial = SerialIntegralValue(a, b, N);
        double endS = GetTime();
        double t_serial = endS - startS;

        fprintf(file, "%lld,1,Serial,%f,1.0,100,%f\n", N, t_serial, resSerial);
        printf("Serial   | Time: %.6f s | Result: %.6f\n", t_serial, resSerial);

        // Parallel calculation
        for (int threads : threadCounts) {
            omp_set_num_threads(threads);

            double startP = GetTime();
            double resParallel = ParallelIntegralValue(a, b, N);
            double endP = GetTime();

            double t_parallel = endP - startP;
            double speedup = t_serial / t_parallel;
            double efficiency = (speedup / threads) * 100.0;

            // Check absolute error between Serial and Parallel
            string status = (abs(resSerial - resParallel) < 1e-8) ? "OK" : "DIFF";

            fprintf(file, "%lld,%d,Parallel,%f,%f,%f,%f\n", N, threads, t_parallel, speedup, efficiency, resParallel);
            printf("Threads %d | Time: %.6f s | Speedup: %.2f | Eff: %.2f%% | %s\n", 
                    threads, t_parallel, speedup, efficiency, status.c_str());
        }
    }

    fclose(file);
    cout << "\n[Success] Results saved to integral_results.csv" << endl;
    return 0;
}