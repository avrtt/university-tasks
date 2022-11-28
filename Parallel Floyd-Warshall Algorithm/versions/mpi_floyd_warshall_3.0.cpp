#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
 
void print(int* graph, int numOfVert) {
 
    for (int i = 0; i < numOfVert; ++i)
    {
        for (int j = 0; j < numOfVert; ++j)
        {
            cout << graph[i*numOfVert + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
 
int main(int argc, char *argv[]) { 
    MPI_Init(&argc, &argv);
    int rank, size; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    MPI_Request request;
    int root = 0;
 
    if (rank == root) {
      double t1,t2;
      vector<int> graph;
      int n, numOfEdges, numOfVert;

      ifstream in("graph.txt");
 
      in >> numOfVert;
      in >> numOfEdges;
      graph.resize(numOfVert*numOfVert);
      for (int i = 0; i < numOfVert; i++) 
      {  
         for (int j = 0; j < numOfVert; j++) 
         {
            graph[i*numOfVert + j] = 0;
         }
      }
 
  for (int i, j, size; in >> i, in >> j, in >> size;)
  {
    graph[(i - 1)*numOfVert + (j - 1)] = graph[(j - 1)*numOfVert + (i - 1)] = size;
  }
  
  print(&graph[0], numOfVert);
  

  MPI_Bcast(&numOfVert, 1, MPI_INT, root, MPI_COMM_WORLD);
  int workProc = size - 1;
  vector<int> rowCounts(workProc);
  

  for(int i = 0; i < workProc - 1; i++) rowCounts[i] = numOfVert/workProc;
  rowCounts[workProc - 1] = numOfVert - ((numOfVert/workProc)*(workProc - 1));
  
  for (int i = 1; i < size; i++) MPI_Send(&rowCounts[i - 1], 1, MPI_INT,i, 0, MPI_COMM_WORLD);
  
  for(int k = 0; k < numOfVert; k++){
    for(int l = 1; l < size; l++){
       MPI_Isend(&graph[k*numOfVert], numOfVert, MPI_INT, l, 0, MPI_COMM_WORLD, &request);
    }
  }

   int num = 0;
   for(int i = 0; i < workProc; i++){
      for(int j = 0; j < rowCounts[i]; j++){
      MPI_Isend(&graph[num*numOfVert], numOfVert, MPI_INT, i+1, 0, MPI_COMM_WORLD, &request);
      num++;
      }
   }
   
   num = 0; 
   for(int i = 0; i < workProc; i++) { 
      for(int j = 0; j < rowCounts[i]; j++) { 
         MPI_Recv(&graph[num*numOfVert], numOfVert, MPI_INT, i+1, root, MPI_COMM_WORLD, &status);
         num++;   
       }
    }

   print(&graph[0], numOfVert);
  }
  else {
   int num;
   MPI_Bcast(&num, 1, MPI_INT, root, MPI_COMM_WORLD);
   int rowCount;
   MPI_Recv(&rowCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);


   vector<int> kRow(num);
   vector<int> rows(rowCount*num);
   
   for(int k = 0; k < num; k++){
      MPI_Recv(&kRow[0], num, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      for(int i = 0; i < rowCount; i++){
          MPI_Recv(&rows[i*num], num, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            
      }
   

      for(int i = 0; i < rowCount; i++){
         for(int j = 0; j < num; j++){
      if(i != j) rows[i*num + j] = min(rows[i*num + j], rows[i*num + k] + kRow[j]);
         }
      }
   
   
      for(int i = 0; i < rowCount; i++){
         MPI_Send(&rows[i*num], num, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
   }
}
 
    MPI_Finalize();
 
    return 0;
}
