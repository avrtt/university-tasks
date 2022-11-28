#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
 
vector<vector<int>> dist;
int n;


void printDist() {
 
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
 
int main(int argc, char *argv[]) {
 
    int my_rank, num_procs, slice, remaind = 0;
 
    MPI_Status status;
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
    remaind = n%(num_procs-1);
    slice = (n-remaind)/(num_procs-1);
 
    if (my_rank == 0) {
      double t1,t2;
      int disable = 0, t = 3;
      int result[t];
      int i, j;
 
      int size, numOfEdges;
 
  ifstream in("graph.txt");
 
  in >> n;
  in >> numOfEdges;
  dist.resize(n);
  for (int i = 0; i < n; i++) 
  { 
    dist[i].resize(n); 
    for (int j = 0; j < n; j++) 
    {
      dist[i][j] = 0;
    }
  }
 
  for (int i, j, size; in >> i, in >> j, in >> size;)
  {
    dist[i - 1][j - 1] = dist[j - 1][i - 1] = size;
  }
 
       printDist();
       t1 = MPI_Wtime();
          for(i = 1; i < num_procs; i++)
              for(int j = 0; j < n; j ++)
                MPI_Send(&dist[j][0], n, MPI_INT, i, 1, MPI_COMM_WORLD);
 
      do {
         MPI_Recv(&result, t, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
 
         if (status.MPI_TAG == 2)
            disable++;
         else
            if (dist[result[1]][result[2]] > result[0])
                dist[result[1]][result[2]] = result[0];
      } while (disable < num_procs-1);
 
       t2 = MPI_Wtime();
 
      printDist();
      printf("Time: %f \n",t2 - t1);
    }
 
    else {
 
        int i, j, k, t = 3;
        int out[t];
        for(int j = 0; j < n; j++)
            MPI_Recv(&dist[j][0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
 
      //  printDist();
        if (my_rank+1 != num_procs)
            remaind = 0;
 
for (k = slice*(my_rank-1); k < slice*(my_rank-1) + slice + remaind; ++k)
for (i = 0; i < n; ++i)
for (j = 0; j < n; ++j)
{
if (i == j)
{
out[0] = -1;
out[1] = i;
out[2] = j;
MPI_Send(&out, t, MPI_INT, 0, 0, MPI_COMM_WORLD);
}
else if (dist[i][j] != 0 && dist[i][k] != 0 && dist[k][j] != 0)
{
out[0] = -1;
out[1] = i;
out[2] = j;
MPI_Send(&out, t, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        MPI_Send(0, 0, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
 
    MPI_Finalize();
 
    return 0;
}