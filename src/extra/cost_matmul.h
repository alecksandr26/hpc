#ifndef COST_MATMUL
#define COST_MATMUL

extern void free_mat(float *ptr);
extern float *alloc_mat(int n);
extern void rand_mat(float *mat, int n, float a);
extern void transpose_mat(float *A, int n);

extern void normal_matmul(float *A, float *B, float *C, int n);
extern void transpose_cache_matmul(float *A, float *B, float *C, int n);
extern void ikj_cache_matmul(float *A, float *B, float *C, int n);
extern void vectorization_matmult(float *A, float *B, float * __restrict__ C, int n);
extern void vectorization_strassen_matmult(float * __restrict__ A, float * __restrict__ B,
					   float * __restrict__ C, int n);

#endif



