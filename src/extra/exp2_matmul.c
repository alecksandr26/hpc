#include <stdio.h>
#include <time.h>
#include <string.h>

#include "cost_matmul.h"

#define N 1024
#define I 2
#define RATE 2.0

int main(void)
{
	for (float f = (float) I; f <= (float) N; f *= RATE) {
		int n = (int) f;
		float *A = alloc_mat(n);
		float *B = alloc_mat(n);
		float *C = alloc_mat(n);

		rand_mat(A, n, 1.0);
		rand_mat(B, n, 1.0);
		
		memset(C, 0.0, n * n * sizeof(float));
		
		clock_t start_time = clock();
		
		transpose_cache_matmul(A, B, C, n);
		
		clock_t end_time = clock();
		double	duration = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
		
		free_mat(A);
		free_mat(B);
		free_mat(C);
		
		printf("%i %f\n", n, duration);
	}

	return 0;
}






