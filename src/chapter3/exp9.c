#include "cost_branching.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N ((long) 1e9)

long A[N];

void init_list(long *arr, size_t n)
{
	for (size_t i = 0; i < n; i++)
		arr[i] = i + 1;
}

int compare(const void *a, const void *b) { return (*(int *) a - *(int *) b); }

int main(void)
{
	init_list(A, N);
	
	for (float f = 1.0; f < (double) N; f += f * 0.01) {
		int n = (int) f;
		int rand_item_index = rand() % n;

		clock_t start_time = clock();
		
		(void) binary_search_expecting(A, n, A[rand_item_index]);
		
		clock_t end_time = clock();
		double	duration = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
		
		printf("%i %f\n", n, duration);
	}

	return 0;
}











