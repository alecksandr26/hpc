#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

/* By default gnu compiler will align this memory */
struct mystruct {
	char c;  // one byte
	int i;   // four bytes
	short s; // two bytes
};

/* In reality this is how the struct will be allocated
  ┌────────────────────────┐
  │                        │
  │C char one byte         │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │I integer four bytes    │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │S short two bytes       │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  ├────────────────────────┤
  │                        │
  │                        │
  └────────────────────────┘
  
  The processor can read each of these members in one transaction.
*/


int main(void)
{
	/* Why aligned memory is faster */
	/* https://stackoverflow.com/questions/381244/purpose-of-memory-alignment */

	/* Alloc the struct */
	struct mystruct s;
	
	printf("s.c -> 0x%lX \n", (unsigned long) &s.c);
	printf("s.i -> 0x%lX \n", (unsigned long)  &s.i);
	printf("0x%lX - 0x%lX = %ld  \n", (unsigned long) &s.i, (unsigned long)  &s.c,
	       (unsigned long) &s.i - (unsigned long) &s.c);
	
	printf("s.s -> 0x%lX \n",(unsigned long)  &s.s);
	printf("0x%lX - 0x%lX = %ld  \n", (unsigned long) &s.s, (unsigned long)  &s.i,
	       (unsigned long) &s.s - (unsigned long) &s.i);


	printf("0x%lX - 0x%lX = %ld  \n", (unsigned long) &s + sizeof(s), (unsigned long)  &s.s,
	       ((unsigned long) &s + sizeof(s)) - (unsigned long) &s.s);


	printf("sizeof(s) = %zu, sizeof(struct mystruct) = %zu\n", sizeof(s), sizeof(struct mystruct));

	/* All data would be alligned aligned */
	
	/* The reason of this:
	   
	   The CPU always reads at its word size (4 bytes on a 32-bit processor), so when you do an unaligned
	   address access — on a processor that supports it — the processor is going to read multiple words.
	   The CPU will read each word of memory that your requested address straddles.
	   This causes an amplification of up to 2X the number of memory transactions required to access the
	   requested data.
	   
	   Because of this, it can very easily be slower to read two bytes than four.
	*/
	
	/* For each item will be aligned in a block 64 bytes */
	char *ptr = aligned_alloc(64, 1024); 
	
	if (!ptr) {
		perror("aligned_alloc()");
		return -1;
	}
	
	puts("\naligned allocated");
	printf("ptr -> 0x%lX \n", (unsigned long) ptr);
	printf("&ptr[64] -> 0x%lX \n", (unsigned long) &ptr[64]);
	
	printf("0x%lX - 0x%lX = %ld  \n", (unsigned long) &ptr[64], (unsigned long)  ptr,
	       (unsigned long) &ptr[64] - (unsigned long)  ptr);
	
	free(ptr);
	
	/* Whats exatly alinged alloc??? */
	/* https://www.reddit.com/r/cpp_questions/comments/ugl7v3/what_does_aligned_alloc_do/ */
	
	/* An example of the power of cache lines */
	size_t n = 1048576;	/* One million */
	
	int *arr = (int *) aligned_alloc(64, n * sizeof(int));
	
	int *arr2 = (int *) malloc(n * sizeof(int));
	int *indexes = (int *) malloc(n * sizeof(int));	
	
	for (int i = 0; i < n; i++)
		arr[i] = i + 1;

	for (int i = 0; i < n; i++)
		arr2[i] = i + 1;


	for (int i = 0; i < n; i++)
		indexes[i] = rand() % n;
	
	/* An aplication of cache line */
	clock_t start_time = clock();


	int sum = 0;
#if 1
	/* A little bit more slower */
	/* for (int i = 0; i < n; i++) */
	/* 	sum += arr[i]; */
	
	for (int i = 0; i < n; i += 16) /* Move 64 bytes */
		for (int j = i; j < i + 16; j++)
			sum += arr[j];
#else
	for (size_t i = 0; i < n; i++)
		sum += arr2[indexes[i]];
#endif

	clock_t end_time = clock();
	double	duration = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
	
	printf("%zu %f\n", n, duration);

	free(arr);
	free(arr2);
	free(indexes);
	
	return 0;
}


