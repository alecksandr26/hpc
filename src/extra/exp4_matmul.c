#include <stdio.h>
#include <time.h>
#include <string.h>

#include "cost_matmul.h"

/* exp1:
  2 0.000001
  4 0.000001
  8 0.000004
  16 0.000018
  32 0.000136
  64 0.001107
  128 0.007865
  256 0.062495
  512 0.613597
  1024 4.913867
*/

/* exp4:
  2 0.000004
  4 0.000003
  8 0.000008
  16 0.000039
  32 0.000220
  64 0.001457
  128 0.010470
  256 0.062551
  512 0.408209
  1024 3.207724
*/

#define N 200
#define I 200
#define RATE 0.1

int main(void)
{
	for (float f = (float) I; f <= (float) N; f += f * RATE) {
		int n = (int) f;
		float *A = alloc_mat(n);
		float *B = alloc_mat(n);
		float *C = alloc_mat(n);

		rand_mat(A, n, 1.0);
		rand_mat(B, n, 1.0);

		memset(C, 0.0, n * n * sizeof(float));
		
		clock_t start_time = clock();
		
		for (int i = 0; i < 10; i++)
			vectorization_matmult(A, B, C, n);
		
		clock_t end_time = clock();
		double	duration = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
		
		free_mat(A);
		free_mat(B);
		free_mat(C);
		
		printf("%i %f\n", n, duration);
	}

	return 0;
}











