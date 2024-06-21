
#include <string.h>
#include <stdlib.h>

float *alloc_mat(int n)
{
	return (float *) malloc(sizeof(float) * n * n);
}

void free_mat(float *ptr)
{
	free(ptr);
}

void rand_mat(float *mat, int n, float a)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			mat[i * n + j] = (float) rand() / (float) (RAND_MAX / a);
}

void transpose_mat(float *A, int n)
{
	/* transpose */
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n; j++) {
			float temp = A[i * n + j];
			A[i * n + j] = A[j * n + i];
			A[j * n + i] = temp;
		}
}

void normal_matmul(float *A, float *B, float *C, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				C[i * n + j] += A[i * n + k] * B[k * n + j];
}



void transpose_cache_matmul(float *A, float *B, float *C, int n)
{
	transpose_mat(B, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				C[i * n + j] += A[i * n + k] * B[j * n + k];
}

void ikj_cache_matmul(float *A, float *B, float *C, int n)
{
	for (int i = 0; i < n; i++)
		for (int k = 0; k < n; k++)
			for (int j = 0; j < n; j++)
				C[i * n + j] += A[i * n + k] * B[k * n + j];
}

// a vector type of 8 elements, float 4 bytes = 32 / 8, in 32 bytes there are 8 float
typedef float vec __attribute__ (( vector_size(32) ));

// a helper function that allocates n vectors and initializes them with zeros
static vec* alloc(int n)
{
	/* Align each 8 float values */
	vec* ptr = (vec*) aligned_alloc(32, 32 * n);
	memset(ptr, 0, 32 * n);
	return ptr;
}

void vectorization_matmult(float * __restrict__ A, float * __restrict__ B,
			   float * __restrict__ C, int n)
{
	int n_v = (n + 7) / 8; // number of 8-element vectors in a row (rounded up) are necessary
	
	/* a[n][n_v][8], sizeof(a) = n * n_v * 32 */
	vec *a = alloc(n * n_v);
	
	/* b[n][n_v][8], sizeof(a) = n * n_v * 32 */
	vec *b = alloc(n * n_v);

	/* Move the the data to the matrices and transpose B */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			/* Fill it vector by vector */
			a[i * n_v + j / 8][j % 8] = A[i * n + j];
			b[i * n_v + j / 8][j % 8] = B[j * n + i]; /* Transpose */
		}
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			/* 8 flaot values initialized in zero */
			vec s = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
			
			/* Do this operation 8 by 8 in the amount of vectos in one column*/
			for (int k = 0; k < n_v; k++) {
				/* s = vec[8] * vec[8] */
				s += a[i * n_v + k] * b[j * n_v + k];
			}

			/* drop the each float value */
			for (int k = 0; k < 8; k++)
				C[i * n + j] += s[k];
		}
	
	free(a);
	free(b);
}


#define VEC_MAT_SIZE 128

void recursive_vec_strassen_matmul(float * __restrict__ A, float * __restrict__ B,
				   float * __restrict__ C, vec * __restrict__ a,
				   vec * __restrict__ b, int n, int N, int n_v)
{
	if (n <= VEC_MAT_SIZE) {
		/* Move the the data to the matrices and transpose B */
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				/* Fill it vector by vector */
				a[i * n_v + j / 8][j % 8] = A[i * n + j];
				b[i * n_v + j / 8][j % 8] = B[j * n + i]; /* Transpose */
			}
	
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				/* 8 flaot values initialized in zero */
				vec s = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
			
				/* Do this operation 8 by 8 in the amount of vectos in one column*/
				for (int k = 0; k < n_v; k++) {
					/* s = vec[8] * vec[8] */
					s += a[i * n_v + k] * b[j * n_v + k];
				}

				/* drop the each float value */
				for (int k = 0; k < 8; k++)
					C[i * n + j] += s[k];
			}
		return;
	}
	
	int k = n / 2;
	
	// c11 = a11 b11 + a12 b21
        recursive_vec_strassen_matmul(A,     B,         C, a, b, k, N, n_v);
        recursive_vec_strassen_matmul(A + k, B + k * N, C, a, b, k, N, n_v);
        
        // c12 = a11 b12 + a12 b22
        recursive_vec_strassen_matmul(A,     B + k,         C + k, a, b, k, N, n_v);
        recursive_vec_strassen_matmul(A + k, B + k * N + k, C + k, a, b, k, N, n_v);
        
        // c21 = a21 b11 + a22 b21
        recursive_vec_strassen_matmul(A + k * N,     B,         C + k * N, a, b, k, N, n_v);
        recursive_vec_strassen_matmul(A + k * N + k, B + k * N, C + k * N, a, b, k, N, n_v);
	
        // c22 = a21 b12 + a22 b22
	recursive_vec_strassen_matmul(A + k * N,     B + k,         C + k * N + k, a, b, k, N, n_v);
        recursive_vec_strassen_matmul(A + k * N + k, B + k * N + k, C + k * N + k, a, b, k, N, n_v);
	
}

	
void vectorization_strassen_matmult(float * __restrict__ A, float * __restrict__ B,
				    float * __restrict__ C, int n)
{
	int n_v = (VEC_MAT_SIZE + 7) / 8; // number of 8-element vectors in a row (rounded up) are necessary
	
	vec *a = alloc(VEC_MAT_SIZE * n_v);
	vec *b = alloc(VEC_MAT_SIZE * n_v);
	
	recursive_vec_strassen_matmul(A, B, C, a, b, n, n, n_v);
	
	free(a);
	free(b);
}




