#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>

const long long SIZE = 2500;

void fill_matrix(double* matrixA, double* matrixB)
{
    for (int i = 0; i < SIZE * SIZE; i += SIZE)
    {
        for (int j = 0; j < SIZE; j++)
        {
            matrixA[i + j] = rand() % 10;
            matrixB[i + j] = rand() % 10;
        }
    }
}

void make_zero_matrix(double* matrixC)
{
    for (int i = 0; i < SIZE * SIZE; i++)
        matrixC[i] = 0;
}

void multiply_matrix1(double* matrixA, double* matrixB, double* matrixC)
{
    for (int i = 1; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for(int k =0; k < SIZE; k++)
                matrixC[i *  SIZE + j] += matrixA[k + i * SIZE] * matrixB[j + k * SIZE];
}


double matrixC_norm(double* matrixC)
{
    double norm;

    for (int i = 0; i < SIZE * SIZE; i++)
        norm = matrixC[i] * matrixC[i];

    norm = sqrt(norm);

    return norm;
}


void multiply_matrix(double* matrixA, double* matrixB, double* matrixC)
{
#pragma omp parallel for schedule(static, 10) num_threads(2)
    for (int i = 1; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                matrixC[i * SIZE + j] += matrixA[k + i * SIZE] * matrixB[j + k * SIZE];
}

void multiply_matrix_max_threads(double* matrixA, double* matrixB, double* matrixC)
{
#pragma omp parallel for schedule(static, 10) num_threads(6)
    for (int i = 1; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                matrixC[i * SIZE + j] += matrixA[k + i * SIZE] * matrixB[j + k * SIZE];
}

int main()
{
    double* matrixA = NULL; 	
    double* matrixB = NULL;
    double* matrixC = NULL;
    double norm;

    {
        if (matrixA == NULL)
            matrixA = new double[SIZE * SIZE];

        if (matrixB == NULL)
            matrixB = new double[SIZE * SIZE];

        if (matrixC == NULL)
            matrixC = new double[SIZE * SIZE];

    }

    fill_matrix(matrixA, matrixB);

    make_zero_matrix(matrixC);

    std::cout << "Consistently" << std::endl;

    {
        auto start_time = std::chrono::steady_clock::now();
        multiply_matrix1(matrixA, matrixB, matrixC);

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << "Time 1: " << elapsed_ms.count() << " ms\n";

        norm = matrixC_norm(matrixC);

        std::cout << "Norm: " << norm << std::endl;
    }

    std::cout << "--------------" << std::endl;

    make_zero_matrix(matrixC);

    std::cout << "2 threads" << std::endl;

    {
        auto start_time = std::chrono::steady_clock::now();
        multiply_matrix(matrixA, matrixB, matrixC);

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << "Time 2: " << elapsed_ms.count() << " ms\n";

        norm = matrixC_norm(matrixC);

        std::cout << "Norm: " << norm << std::endl;
    }

    std::cout << "--------------" << std::endl;

    make_zero_matrix(matrixC);

    std::cout << "Max threads" << std::endl;

    {
        auto start_time = std::chrono::steady_clock::now();
        multiply_matrix_max_threads(matrixA, matrixB, matrixC);

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << "Time 3: " << elapsed_ms.count() << " ms\n";

        norm = matrixC_norm(matrixC);

        std::cout << "Norm: " << norm << std::endl;
    }


    {
        if (matrixA)
        {
            delete[] matrixA;
            matrixA = NULL;
        }

        if (matrixB)
        {
            delete[] matrixB;
            matrixB = NULL;
        }

        if (matrixC)
        {
            delete[] matrixC;
            matrixC = NULL;
        }
    }

    return 0;
}