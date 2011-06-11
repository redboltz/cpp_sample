// g++ -fopenmp 
#include <iostream>
#include <omp.h>

int main() {

#ifdef _OPENMP
	std::cout<< "The number of processors is " << omp_get_num_procs() << std::endl;
#endif
	int const N = 10000;
	int a[N];
	int b[N] = { 1 };
	int c[N] = { 2 };
#pragma omp parallel for
	for(int i = 0; i < N; ++i) {
		a[i] = b[i] + c[i];
	}
}
