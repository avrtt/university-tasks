#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
using namespace std;

bool comp(int a, int b) 
{
    return a < b;
}

int main()
{
    cout << "FIRST" << endl;
    stack<int> s;
    int num;

    for (int i = 0; i < 5; i++)
    {
        cin >> num;
        s.push(num);
    }

    cout << "out" << endl;

    for (int i = 0; i < 5; i++)
    {
        cout << s.top() << " ";
        s.pop();
    }
    


    cout << endl << endl << "SECOND" << endl;
    vector<int> a = { 1,7,3,3,1,5,2,2,6 };

    int size = a.size();
    cout << "in: ";
    for (int i = 0; i < size; i++) cout << a[i] << " ";
    cout << endl;

    sort(a.begin(), a.end(), comp);

    a.erase(unique(a.begin(), a.end()), a.end());

    size = a.size();
    cout << "out: ";
    for (int i = 0; i < size; i++) cout << a[i] << " ";



    cout << endl << endl << "THIRD" << endl;
    vector<vector<int>> v(3, vector<int>());
    int i, j;

    while (true) 
    {
        cin >> i;
        cin >> j;
        
        cout << endl;
        v[i].push_back(j);
        if (i == 0 && j == 0) break;
    }

    int vSize = v.size();
    for (int i = 0; i < vSize; i++)
    {
        size = v[i].size();
        cout << "vector " << i << ": ";
        for (int j = 0; j < size; j++) cout << v[i][j] << " ";
        cout << endl;
    }
}
