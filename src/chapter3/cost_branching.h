#ifndef COST_BRANCHING
#define COST_BRANCHING

#include <stddef.h>

extern void rand_list(int *A, int N);
extern int normal(int *A, int N);
extern int expecting_true(int *A, int N);
extern int expecting_false(int *A, int N);
extern int branch_less(int *A, int N);
extern int vectorize_loop_branch_less(int *A, int N);
extern int vectorize_loop_expecting_true(int *A, int N);
extern long binary_search(long *arr, size_t n, long item);
extern long binary_search_branch_less(long *arr, size_t n, long item);
extern long binary_search_expecting(long *arr, size_t n, long item);

#endif


