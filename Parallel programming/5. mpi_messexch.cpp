#include <iostream>
#include <cmath>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv) 
{
    int num_threads = 3;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


// компьютер с рангом != 0 создает вектор и находит норму, после чего передает её компьютеру с рангом = 0.

    if (rank != 0)
    {
        int n = 10000; // длина вектора
        double* vector = new double[n];

        #pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < n; i++)
        {
            vector[i] = rand() % 10 + 1;
        }

        // нахождение нормы
        double norm = 0;

        #pragma omp parallel for num_threads(num_threads) reduction(+:norm)
        for (int i = 0; i < n; i++)
        {
            norm += vector[i] * vector[i];
        }

        norm = sqrt(norm);

        MPI_Send(& norm, 1 , MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        MPI_Finalize();
        return 0;
    }

    // компьютер с рангом = 0 принимает нормы от других и выводит их
    if (rank == 0)
    {
        double vec = 0;
        MPI_Request req;
        MPI_Status status;

            for (int i = 1; i < size ; i++)
            {
                MPI_Irecv(&vec, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &req); // запрос на сообщение

                for (int flag = 0;!flag;) // ожидание сообщения
                {
                    MPI_Test(&req, &flag, &status);

                    // если сообщение доставлено, то:
                    if (flag)
                    {
                        MPI_Irecv(&vec, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &req);
                        cout << endl << "Process " << i << ", norm: " << vec << endl;
                    }
                }
            }

        MPI_Finalize();
        return 0;
    }

    MPI_Finalize();
    return 0;
}
