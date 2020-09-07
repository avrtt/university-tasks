#include <iostream>
#include <omp.h>

using namespace std;

void HelloWorld1() {
#pragma omp parallel
    cout << 'Hello World ' << omp_get_thread_num() << 'Number of threads: ' << omp_get_num_threads() << endl;
}

void HelloWorld2() {
#pragma omp parallel sections
    {
    #pragma omp section
    cout << 'Hello world ' << omp_get_thread_num() << 'Number of threads: ' << omp_get_num_threads() << endl;
    #pragma omp section
    cout << 'Hello world ' << omp_get_thread_num() << 'Number of threads: ' << omp_get_num_threads() << endl;
    #pragma omp section
    cout << 'Hello world ' << omp_get_thread_num() << 'Number of threads: ' << omp_get_num_threads() << endl;
    }
}

void loop() {
    const int n = 20;
    int a[n];

    #pragma omp parallel for schedule(static, 10)
    for (int i = 0; i < n; i++) {
        a[i] = omp_get_thread_num();
    }
    for (int i = 0; i < n; i++) {
    cout << a[i];
    }
}

int main() {
    int max = omp_get_max_threads();
    HelloWorld1();
    HelloWorld2();
    loop();
    cout << max << endl;
}
