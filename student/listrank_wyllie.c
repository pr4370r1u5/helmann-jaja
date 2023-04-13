#include "listutils.h"
#include <omp.h>
#include <stdlib.h>
#include <math.h>


void parallelListRanks (long head, const long *next, long *rank, size_t n)
{
	/* source : class material */
	/* source : freecodecamp.org : Pointers in C / C++ https://www.youtube.com/watch?v=zuegQmMdy8M
	
	/* The first parameter is the head node location. */
	/* The second parameter is an array of the next locations. */
	/* The third is an array for your list rankings. */
	/* The fourth parameter is the size of the arrays. */

	int primary_limit = ceil(log2(n));

	long *rank_begin = rank;

	long *n1 = malloc(n * sizeof(long));
	long *n2 = malloc(n * sizeof(long)); 
	long *rank2 = malloc(n * sizeof(long)); 

	long *n1_begin = n1;
	long *n2_begin = n2;
	
	long *rank2_begin = rank2;
	
	// load *next values into n1 and n2 & create rank values
	
	#pragma omp parallel for private(n1, n2, rank, rank2)
	for (int w=0; w<n ;w++)
	{	n1 = n1_begin+w; *n1 = *(next+w);  
		n2 = n2_begin+w; *n2 = *n1; //*(next+w);
		
		rank = rank_begin+w; *rank = 1; 
		rank2 = rank2_begin+w; *rank2 = 1;
	}
	*(rank_begin+head) = 0; *(rank2_begin+head) = 0;

		
	// main PLR loop
	//sequential
	for (int i=0; i<primary_limit; i++) 
	{
		/* updateRanks :: R_2 = updateRanks (R_1, R_2, N_1); */
		#pragma omp parallel for private(n1, n2, rank2) 
		for (int x = 0; x<n; x++) //pointer agnostic
		{	n1 = n1_begin+x; // point to N[x]			
			n2 = n2_begin+x; //jumpList
			
			if (*n1 >= 0)
			{	rank2 = rank2_begin + (*n1); // point to location R2[N[x]]
				
				*rank2 = *(rank_begin+x) + (*(rank_begin+(*n1))); // set value : R2[N[j]] = R1[x] + R1[N[x]]
				//jumpList
				*n2 = *(n1_begin + (*n1)); // N2[j] = N1[N1[j]]
			}
			else
			{	*n2 = (-1); //jumpList
			}
				
		}
		
		#pragma omp parallel for private(rank,rank2)
		for (long z = 0; z<n; z++) //pointer agnostic
		{
			rank = rank_begin+z;
			rank2 = rank2_begin+z;
			*rank = *rank2;
			
			
		}
		

		/* alternate pointers */
		n1 = n2_begin; n2 = n1_begin;
		n1_begin = n1; n2_begin = n2;

		//only copy array in one direction. can it be done with pointers?
		
		
			
	}

free(n1_begin); free(rank2_begin); free(n2_begin);

rank = rank_begin;
}

