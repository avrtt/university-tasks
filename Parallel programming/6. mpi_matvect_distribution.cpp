#include <iostream>
#include <mpi.h>

using namespace std;
const int n = 5;

void InitVec(double* vector)
{
    for (int i = 0; i < n; i++)
    {
        vector[i] = i;
    }
}

void InitMat(double* matrix)
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            matrix[i * n + j] = (i * n) + j;
        }
    }
}

void Distribution(int* sendcounts, int size)
{
    int rows = n;

    for(int i = 0; i < size; i++) sendcounts[i] = 0;

    while(true)
    {
        if(rows < size)
        {
            while(rows > 0)
            {
                int i = 0;
                sendcounts[i] += n;
                i++;
                rows--;
            }
        }
        else
        {
            for(int i = 0; i<size;i++) sendcounts[i] += (rows/size)*n;
            rows = rows%size;
        }
        if (rows == 0) break;
    }
}

void multiply(double* rows, double* vector, double* component,int size)
{
    for(int i = 0; i<size; i++)
    {
        double sum = 0;
        for(int j = 0; j<n;j++)
        {
            sum += vector[j]*rows[i*n+j];
        }
        component[i] = sum;
    }
}

int main(int argc, char** argv)
{
    int rank, size;
    double* vector = new double[n];
    double* matrix = new double[n*n];
    double* result = new double[n];
    int* displs;
    int* sendcounts;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    displs = new int[size];
    sendcounts = new int[size];
    
    if(rank == 0)
    {
        InitVec(vector);
        InitMat(matrix);
    }

    Distribution(sendcounts, size);
    int displacement = 0;
    displs[0] = 0;

    for(int i = 1; i < size ; i++)
    {
        displacement += sendcounts[i-1];
        displs[i] = displacement;
    }

    double* rows = new double[n*sendcounts[rank]];

    MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(matrix, sendcounts, displs, MPI_DOUBLE, rows, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double* component = new double[sendcounts[rank]];
    multiply(rows, vector, component, sendcounts[rank]);

    MPI_Gatherv(component, sendcounts[rank], MPI_DOUBLE, result, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(int i = 0; i < n;i++) cout<<result[i]<<endl;
    }
    
    MPI_Finalize();
    return 0;
}
