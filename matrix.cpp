
// Khai bao thu vien 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <omp.h>
#include <time.h>
using namespace std;

// Ham tinh thoi gian chay
double LiToDouble(LARGE_INTEGER x)
{
    double result =
        ((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
    return result;
}
double GetTime()
{
    LARGE_INTEGER lpFrequency, lpPerfomanceCount;
    QueryPerformanceFrequency(&lpFrequency);
    QueryPerformanceCounter(&lpPerfomanceCount);
    return LiToDouble(lpPerfomanceCount) / LiToDouble(lpFrequency);
}

// Ham khoi tao ma tran vuong n*n va vector voi kich co n
void RandomDataInitialization(double *pMatrix, double *pVector, int Size)
{
    int i, j; // Loop variables
    srand(time(NULL));
    for (i = 0; i < Size; i++)
    {
        pVector[i] = rand() / (double)(1000);
        for (j = 0; j < Size; j++)
            // Dam bao cho gia tri khong vuot qua 1000
            pMatrix[i * Size + j] = rand() / (double)(1000);
    }
}

void ProcessInitialization(double *&pMatrix, double *&pVector, int &Size)
{
    // Dam bao cho viec nhap kich co ma tran la lon hon 0
    do
    {
        printf("\nChosen objects size = ");
        scanf("%d", &Size);
        if (Size <= 0)
            printf("\nSize of objects must be greater than 0!\n");
    } while (Size <= 0);
    
    
    // Cap phat bo nho dong cho ma tran va cho vector
    pMatrix = new double[Size * Size];
    pVector = new double[Size];
    RandomDataInitialization(pMatrix, pVector, Size);
}

// Ham tinh gia tri vector nhan ma tran theo tuan tu
void SerialResultCalculation(double *pMatrix, double *pVector, double *pResult, int Size)
{
    int i, j; // Loop variables
    for (i = 0; i < Size; i++)
    {
        pResult[i] = 0;
        for (j = 0; j < Size; j++)
            pResult[i] += pMatrix[i * Size + j] * pVector[j];
    }
}

// Ham tinh gia tri vector nhan ma tran theo song song
// Thuat toan: song song cho vong for dau tien, tuc la cac gia tri o hang i cua ket qua duoc tinh song song.
void ParallelResultCalculation(double *pMatrix, double *pVector, double *pResult, int Size)
{
    int i, j; // Loop variables
    #pragma omp parallel for private(j)
    for (i = 0; i < Size; i++)
    {
        pResult[i] = 0;
        for (j = 0; j < Size; j++)
            pResult[i] += pMatrix[i * Size + j] * pVector[j];
    }
}

// Kiem tra ket qua xem tính toán song song và tuan tu co giong nhau khong
void TestResult(double *pMatrix, double *pVector, double *pResult,
                int Size)
{
    // Luu tru ket qua tinh toan tam thoi o bo nho dem 
    double *pSerialResult;
    int equal = 0; 
    int i;         
    pSerialResult = new double[Size];
    SerialResultCalculation(pMatrix, pVector, pSerialResult, Size);
    for (i = 0; i < Size; i++)
    {
        if (pResult[i] != pSerialResult[i])
        {
            equal = 1;
        }
    }
    if (equal == 1)
        printf("\nThe results of serial and parallel algorithms "
               "are NOT identical. Check your code.");
    else
        printf("\nThe results of serial and parallel algorithms are "
               "identical.");
    delete[] pSerialResult;
}

// Ham giai phong bo nho
void ProcessTermination(double *pMatrix, double *pVector)
{
    delete[] pMatrix;
    delete[] pVector;
}

// Ham chinh 
int main()
{
    double *pMatrix; // The first argument - initial matrix
    double *pVector; // The second argument - initial vector
    double *pResult_Serial; // Result vector for matrix-vector multiplication
    double *pResult_Parallel;
    int Size, p;        // Sizes of initial matrix and vector
    double Start, Finish, Duration_Serial, Duration_Parallel;
    double Efficiency;
    printf("Parallel matrix-vector multiplication program\n");
    // Cap phat bo nho dong cho matran va vector
    ProcessInitialization(pMatrix, pVector, Size);
    pResult_Serial = new double[Size];
    pResult_Parallel = new double[Size];

    // Tinh thoi gian chay tuan tu
    Start = GetTime();
    SerialResultCalculation(pMatrix, pVector, pResult_Serial, Size);
    Finish = GetTime();
    Duration_Serial = Finish - Start;


    // Tinh thoi gian chay song song
    Start = GetTime();
    ParallelResultCalculation(pMatrix, pVector, pResult_Parallel, Size);
    Finish = GetTime();
    Duration_Parallel = Finish - Start;
    #pragma omp parallel
        p = omp_get_num_threads();
    Efficiency = Duration_Serial / (Duration_Parallel * p);
    double SpeedUp = Duration_Serial / (Duration_Parallel);
    
    // In ket qua chay va chi so E
    printf("\n\nSo luong su dung: %d\n",p);
    printf("\n\nThoi gian chay tuan tu: %f\n", Duration_Serial);
    printf("\n\nThoi gian chay song song: %f\n", Duration_Parallel);
    printf("\n\nEfficiency: %f%\n", Efficiency * 100);
    printf("\n\nSpeedUp: %f\n", SpeedUp);

    // Giai phong bo nho
    delete[] pResult_Serial;
    delete[] pResult_Parallel;
    ProcessTermination(pMatrix, pVector);
    
    return 0;
}
