// ==================================================
// Parallel Vector Dot Product using OpenMP
// Course: Parallel Computing
// ==================================================

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <omp.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>


using namespace std;

// --------------------------------------------------
// High-resolution timer 
// --------------------------------------------------
double LiToDouble(LARGE_INTEGER x)
{
    return ((double)x.HighPart) * 4.294967296E9 + (double)x.LowPart;
}

double GetTime()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return LiToDouble(counter) / LiToDouble(frequency);
}

// --------------------------------------------------
// Initialize two vectors with random values
// --------------------------------------------------
void RandomVectorInitialization(double* v1, double* v2, int size)
{
    for (int i = 0; i < size; i++)
    {
        v1[i] = rand() % 1000 + (double)rand() / RAND_MAX;
        v2[i] = rand() % 1000 + (double)rand() / RAND_MAX;
    }
}

// --------------------------------------------------
// Serial dot product
// --------------------------------------------------
double SerialVectorDotProduct(double* v1, double* v2, int size)
{
    double result = 0.0;
    for (int i = 0; i < size; i++)
        result += v1[i] * v2[i];
    return result;
}

// --------------------------------------------------
// Parallel dot product using OpenMP
// --------------------------------------------------
double ParallelVectorDotProduct(const vector<double>& v1,
                              const vector<double>& v2, int size)
{
    double result = 0.0;

    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < size; i++)
        result += v1[i] * v2[i];

    return result;
}

// --------------------------------------------------
// Compare serial and parallel results
// --------------------------------------------------
void CheckResult(double serialResult, double parallelResult)
{
        double diff = fabs(serialResult - parallelResult);
        if (diff > 1e-8)
            printf("\n[WARNING] Absolute error between serial and parallel results: %.15e\n", diff);
        else
            printf("\n[OK] Serial and parallel results are identical.\n");
}


int main()
{
    vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    vector<int> threadCounts = {2, 4};

    ofstream file("vector_dot_product.csv");
    file << "Size,ExecutionType,Threads,Time,Speedup,Efficiency\n";

    printf("=== Parallel Vector Dot Product (OpenMP) ===\n");

    srand(123); // fixed seed

    for (int size : sizes)
    {
        printf("\n--- Vector size = %d ---\n", size);

        
        vector<double> v1(size), v2(size);
       

        
        RandomVectorInitialization(v1.data(), v2.data(), size);

        // ===== SERIAL RUN =====
        double start = GetTime();
        double serialResult = SerialVectorDotProduct(v1.data(), v2.data(), size);
        double finish = GetTime();
        double t_serial = finish - start;

        file << size << ",Serial,1,"
             << t_serial << ",1.0,100\n";

        printf("Serial time: %f\n", t_serial);

        // ===== PARALLEL RUNS =====
        for (int threads : threadCounts)
        {
            omp_set_num_threads(threads);

            start = GetTime();
            double parallelResult = ParallelVectorDotProduct(v1, v2, size);
            finish = GetTime();
            double t_parallel = finish - start;

            double speedup = t_serial / t_parallel;
            double efficiency = speedup / threads * 100.0;

              file << size << ",Parallel," << threads << ","
         << t_parallel << ","
         << speedup << ","
         << efficiency << "\n";


            printf("Threads %d | Speedup %.2f | Efficiency %.2f%%\n",
                   threads, speedup, efficiency);
        }

        
    }

    file.close();
    printf("\nResults saved to vector_dot_product.csv\n");
    return 0;
}
