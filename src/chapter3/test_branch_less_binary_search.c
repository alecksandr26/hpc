#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cost_branching.h"

#define N ((long) 1e9)

long A[N];

void init_list(long *arr, size_t n)
{
	for (size_t i = 0; i < n; i++)
		arr[i] = i + 1;
}

int main(void)
{
	init_list(A, N);
	
	for (float f = 1.0; f < (double) N; f += f * 0.01) {
		int n = (int) f;
		int rand_item_index = rand() % n;

		long expect = binary_search(A, n, A[rand_item_index]);
		long res = binary_search_branch_less(A, n, A[rand_item_index]);

		assert(expect == res);
	}

	return 0;
}


