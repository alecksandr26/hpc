#include <stdio.h>
#include <time.h>

#include "cost_branching.h"

#define N ((int) 1e6)

int main(void)
{	
	for (float f = 1.0; f < (double) N; f += f * 0.01) {
		int n = (int) f;
		int A[n];
		
		rand_list(A, n);
		
		clock_t start_time = clock();
		(void) expecting_false(A, n);

		clock_t end_time = clock();
		double	duration = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
		
		printf("%i %f\n", n, duration);
	}

	return 0;
}

