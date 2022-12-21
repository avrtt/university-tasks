#include <stdio.h>
 
extern double __stdcall SolveByChord(int* out, double epsilon);
 
double InitializeEpsilon()
{
	double epsilon;
 
	printf_s("let epsilon is ");
	scanf_s("%lf", &epsilon);
 
	return epsilon;
}
 
void main() 
{
	const char* equation = "'x + ln(x + 0.5) - 0.5 = 0'";
	printf_s("Solving the %s by the chord method\n", equation);
 
	double epsilon = InitializeEpsilon();
 
	int iterations = 0;
 
	double result = SolveByChord(&iterations, epsilon);
	printf_s("\nResult: %.10lf, epsilon: %.10lf\nIterations: %d", result, epsilon, iterations);
}
