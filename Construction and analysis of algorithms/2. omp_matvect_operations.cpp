#include <iostream>
#include <omp.h>
#include <ctime>
#include <cmath>

using namespace std;

const int n = 15;
double alpha = n;

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

double multiplyScalar(double* vector1, double* vector2) {
	double scalarProduct = 0;
	double startTime = clock();
	#pragma omp parallel for reduction(+:scalarProduct)
	for (int i = 0; i < n; i++) {
		scalarProduct += vector1[i] * vector2[i];
	}
	double endTime = clock();
	cout << "Multiplication time: " << (endTime - startTime) / CLOCKS_PER_SEC << endl;
	return scalarProduct;
}

double vectorLength(double* vector) {
	double vectorLength = 0;
#pragma omp parallel for reduction(+:vectorLength)
	for (int i = 0; i < n; i++) {
		vectorLength += vector[i] * vector[i];
	}
	return sqrt(vectorLength);
}

void multiplyMatVect(double* vector, double* matrix, double* vectorResult) {
	double startTime = clock();
#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		double sum = 0;
		for (int j = 0; j < n; j++) {
			sum += matrix[i * n + j] * vector[j];
		}
		vectorResult[i] = sum;
	}
	double endTime = clock();
	cout << "Multiplication time: " << (endTime - startTime) / CLOCKS_PER_SEC << endl;
}


void linComb(double* vector1, double* vector2, double* vectorResult) {
	double startTime = clock();
#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		vectorResult[i] = vector1[i] + (alpha * vector2[i]);
	}
	double endTime = clock();
	cout << "Linear combination time: " << (endTime - startTime) / CLOCKS_PER_SEC << endl;
}

int main() {
	omp_set_num_threads(2);

	double* x;
	x = new double[n];
	double* y;
	y = new double[n];
	double* A;
	A = new double[n * n];

	initializeVector(x);
	initializeVector(y);
	initializeMatrix(A);

	cout << "Scalar product : " << multiplyScalar(x, y) << endl;
	cout << endl;

	double* vectorResult = new double[n];
	multiplyMatVect(x, A, vectorResult);
	cout << "Vector result: " << endl;
	for (int i = 0; i < n; i++) {
		cout << vectorResult[i] << endl;
	}
	cout << "Vector length: " << vectorLength(vectorResult) << endl;
	cout << endl;
	delete[] vectorResult;
	vectorResult = NULL;

	vectorResult = new double[n];
	linComb(x, y, vectorResult);
	for (int i = 0; i < n; i++) {
		cout << vectorResult[i] << endl;
	}
	delete[] vectorResult;
	vectorResult = NULL;

	delete[] x;
	x = NULL;
	delete[] y;
	y = NULL;
	delete[] A;
	A = NULL;
}
