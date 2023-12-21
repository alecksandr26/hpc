
/* chapter 3: https://en.algorithmica.org/hpc/pipelining/branching/#an-experiment */

/* To dissamble code:
   objdump -S a.out
*/

#include <stdlib.h>

#define PROBAILITY 90

void rand_list(int *A, int N)
{
	for (int i = 0; i < N; i++)
		A[i] = rand() % (100 + 1);
}

int normal(int *A, int N)
{

	/* to don't vectorize the loop */
	volatile int S = 0;

	for (int i = 0; i < N; i++)
		if (A[i] < PROBAILITY)
			S += A[i];
	
	return S;
}

/* We can use likeliness attributes and compiler-specific
   intrinsics to hint at the likeliness of branches: __builtin_expect_with_probability in GCC
   and __builtin_unpredictable in Clang. */

int expecting_true(int *A, int N)
{

	/* to don't vectorize the loop */
	volatile int S = 0;

	for (int i = 0; i < N; i++)
		if (__builtin_expect(A[i] < PROBAILITY, 1))
			S += A[i];
	
	return S;
}

int expecting_false(int *A, int N)
{
	/* to don't vectorize the loop */
	volatile int S = 0;

	for (int i = 0; i < N; i++)
		if (__builtin_expect(A[i] < PROBAILITY, 0))
			S += A[i];
	
	return S;
}

int branch_less(int *A, int N)
{
	/* to don't vectorize the loop */
	volatile int S = 0;

	/* This is the faster version */
	for (int i = 0; i < N; i++)
		S += A[i] < PROBAILITY ? A[i] : 0;
	
	return S;
}

int vectorize_loop_branch_less(int *A, int N)
{
	/* to vectorize the loop */
	int S = 0;
	
	/* Data-parallel programming. Branchless programming is very important for SIMD applications
	   because they don’t have branching in the first place. */

	/* Compile it with -O3 -mavx  to vectorize*/
	
	for (int i = 0; i < N; i++)
		S += A[i] < PROBAILITY ? A[i] : 0;


	return S;
}



int vectorize_loop_expecting_true(int *A, int N)
{
	/* to vectorize the loop */
	int S = 0;
	
	/* Data-parallel programming. Branchless programming is very important for SIMD applications
	   because they don’t have branching in the first place. */

	/* Compile it with -O3 -mavx  to vectorize*/
	
	for (int i = 0; i < N; i++)
		if (__builtin_expect(A[i] < PROBAILITY, 1))
			S += A[i];


	return S;
}

long binary_search(long *arr, size_t n, long item)
{
	int l, r, m;

	l = 0;
	r = n;
	while (l <= r) {
		m = ((r - l) / 2) + l;

		if (item == arr[m]) return m;
		else if (item < arr[m]) r = m;
		else l = m;
	}

	return -1;
}


long binary_search_expecting(long *arr, size_t n, long item)
{
	int l, r, m;

	l = 0;
	r = n;
	while (l <= r) {
		m = ((r - l) / 2) + l;

		if (__builtin_expect(item == arr[m], 0)) return m;
		else if (item < arr[m]) r = m;
		else l = m;
	}

	return -1;
}

long binary_search_branch_less(long *arr, size_t n, long item)
{
	long *ptr = arr;
	int m;
	
	while (n > 1) {
		m = n / 2;
		arr += (arr[m - 1] < item) * m;
		n -= m;
	}
	
	return arr - ptr;
}







