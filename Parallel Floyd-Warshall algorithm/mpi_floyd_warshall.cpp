#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

void createTransClos(bool** matrix, unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
        for (unsigned short s = 0; s < size; s++)
            for (unsigned short t = 0; t < size; t++)
                if (matrix[s][i] && matrix[i][t]) matrix[s][t] = true;
}


void createTransClosParallel(bool** matrix, unsigned short size, int threads)
{
    for (unsigned short i = 0; i < size; i++)
#pragma omp parallel num_threads(threads)
#pragma omp for
        for (unsigned short s = 0; s < size; s++)
            for (unsigned short t = 0; t < size; t++)
                if (matrix[s][i] && matrix[i][t]) matrix[s][t] = true;
}


double getTime(struct timespec* start, struct timespec* finish)
{
    long int sec = finish->tv_sec - start->tv_sec;
    long int ns = finish->tv_nsec - start->tv_nsec;
    return sec + (double)ns / 1000000000;
}


void OutMatrix(bool** matrix, unsigned short size)
{
    for (unsigned short y = 0; y < size; y++)
    {
        for (unsigned short x = 0; x < size; x++)
            if (matrix[x][y]) cout << "1 ";
            else cout << "0 ";
        cout << endl;
    }
}

int main()
{
    struct timespec cl_start, cl_end;

    ifstream in("1000.txt");

    unsigned short numOfVert, sizeOfEdges;
    unsigned short temp;
    in >> numOfVert;
    in >> sizeOfEdges;

    bool** matrix = new bool*[numOfVert];
    for (unsigned short i = 0; i < numOfVert; i++)
        matrix[i] = new bool[numOfVert];

    for (int i = 0; i < numOfVert; i++)
    {
        for (int j = 0; j < numOfVert; j++)
        {
            matrix[i][j] = false;
        }
    }
    for (int i, j; in >> i, in >> j;)
    {
        matrix[i - 1][j - 1] = true;
    }
       

    OutMatrix(matrix, numOfVert);

    cout << "cons/par?" << endl;
    string input;
    cin >> input;
    cout << endl;

    if (input == "cons")
    {
        clock_gettime(CLOCK_REALTIME, &cl_start);
        createTransClos(matrix, numOfVert);
    }
    else if (input == "par")
    {
        int numOfThreads;

        cout << "num of threads: ";
        cin >> numOfThreads;

        clock_gettime(CLOCK_REALTIME, &cl_start);
        createTransClosParallel(matrix, numOfVert, numOfThreads);
    }

    OutMatrix(matrix, numOfVert);

    clock_gettime(CLOCK_REALTIME, &cl_end);
    cout << "\nTime: ";
    cout << fixed << setprecision(6) << getTime(&cl_start, &cl_end) << " sec." << endl;


    return 0;
}


