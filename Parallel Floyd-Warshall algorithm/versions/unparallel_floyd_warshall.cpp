#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

vector<vector<int>> graph;

void Print() 
{
  for (int i = 0; i < graph.size(); ++i)
  {
    for (int j = 0; j < graph.size(); ++j)
    {
      cout << graph[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void Floyd() 
{
  double start = clock();
  for (int k = 0; k < graph.size(); k++)
  {
    for (int i = 0; i < graph.size(); i++)
    {
      for (int j = 0; j < graph.size(); j++)
      {
        if (i == j) graph[i][j] = 0;
        else if (graph[i][j] != 0 && graph[i][k] != 0 && graph[k][j] != 0) graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
      }
    }
  }
  cout << "Time: " << (clock() - start) / CLOCKS_PER_SEC << endl;
}

int main()
{
  int size, numOfEdges;

  ifstream in("graph.txt");
  in >> size;
  in >> numOfEdges;
  graph.resize(size);
  for (int i = 0; i < size; i++) 
  { 
    graph[i].resize(size); 
    for (int j = 0; j < size; j++) 
    {
      graph[i][j] = 0;
    }
  }

  for (int i, j, length; in >> i, in >> j, in >> length;)
  {
    graph[i - 1][j - 1] = graph[j - 1][i - 1] = length;
  }

  Print();
  Floyd();
}