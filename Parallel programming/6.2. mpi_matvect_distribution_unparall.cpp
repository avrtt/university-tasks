#include <iostream>

using namespace std;

const int n = 5;

void initializeVector(double* vector) {

	for (int i = 0; i < n; i++) {

		vector[i] = i;

	}

}

void initializeMatrix(double* matrix) {

	for (int i = 0; i < n; i++) {

		for (int j = 0; j < n; j++) {

			matrix[i * n + j] = (i * n) + j;

		}

	}

}

void multiplyMatVect(double* vector, double* matrix, double* vectorResult) {

	

	for (int i = 0; i < n; i++) {

		double sum = 0;

		for (int j = 0; j < n; j++) {

			sum += matrix[i * n + j] * vector[j];

		}

		vectorResult[i] = sum;

	}
}


int main() {

	double* x;

	x = new double[n];

	double* A;

	A = new double[n * n];

	initializeVector(x);

	initializeMatrix(A);

	double* vectorResult = new double[n];

	multiplyMatVect(x, A, vectorResult);

	cout << "Vector result: " << endl;

	for (int i = 0; i < n; i++) {

		 cout << vectorResult[i] << endl;

	}

	delete[] vectorResult;

	vectorResult = NULL;

	delete[] x;

	x = NULL;

	delete[] A;

	A = NULL;
}