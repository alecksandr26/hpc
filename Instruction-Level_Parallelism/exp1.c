
/* chapter 3: https://en.algorithmica.org/hpc/pipelining/branching/#an-experiment */

/* To dissamble code:
   objdump -S a.out
 */

/* To do compile it:
   cc -O2 -march=native exp1.c
   clang -O2 -march=native exp1.c
*/

#include <stdio.h>
#include <stdlib.h>

#define N ((int) 1e6)		/* One million */
#define P 10

int main(void)
{
	int a[N];
	
	for (int i = 0; i < N; i++)
		a[i] = rand() % 100;
	
	/* without volatile it vectorized the code */
	/* https://stackoverflow.com/questions/1422149/what-is-vectorization */
	
	volatile int s;
	/* int s; */
	
	s = 0;
	for (int i = 0; i < N; i++) {
		/* if (a[i] < P) */
		
		/* High probability to happend */
		/* if (__builtin_expect(a[i] < P, 1)) */
		
		/* Low probability to happend */
		if (__builtin_expect(a[i] < P, 0)) 
			s += a[i];
	}

	/* The number */
	printf("%i\n", s);
	
	return 0;
}
