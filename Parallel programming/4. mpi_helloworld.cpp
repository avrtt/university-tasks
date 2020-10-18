#include <iostream>
#include "/usr/include/openmpi/mpi.h" // подключение библиотеки
using namespace std;
 
int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    cout << "Hello, I'm process " << rank << " of " << size << " processes" << endl;
 
    MPI_Finalize();
    return 0;
}
