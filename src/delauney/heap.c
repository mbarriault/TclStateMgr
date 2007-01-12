/* This code is part of the public domain
 * Sweep2 algorithm, and
 * was written by:
 * Author:
 *   Steve Fortune
 * Reference:
 *   Steve Fortune
 *   A Sweepline Algorithm for Voronoi Diagrams
 *   Algorithmica, Volume 2, pages 153-174, 1987.
 * Webpage:
 *   http://www.csit.fsu.edu/~burkardt/cpp_src/sweep2/sweep2.html
 * It is made freely avaliable through the Netlib Repository
 * (see http://www.netlib.org)
 * and can be downloaded at http://www.netlib.org/voronoi/sweep2 .
 * 
 */
#include "defs.h"
#include "heap.h"
#include "vor_memory.h"
#include "geometry.h"


void PQinsert(struct Halfedge *he, struct Site *v, float offset)
{
	struct Halfedge *last, *next;

	he -> vertex = v;
	ref(v);
	he -> ystar = v -> coord.y + offset;
	last = &PQhash[PQbucket(he)];
	while ((next = last -> PQnext) != (struct Halfedge *) NULL &&
			(he -> ystar  > next -> ystar  ||
			 (he -> ystar == next -> ystar && v -> coord.x > next->vertex->coord.x)))
	{
		last = next;
	}
	he -> PQnext = last -> PQnext; 
	last -> PQnext = he;
	PQcount += 1;
}

void PQdelete(struct Halfedge *he)
{
	struct Halfedge *last;

	if(he ->  vertex != (struct Site *) NULL)
	{
		last = &PQhash[PQbucket(he)];
		while (last -> PQnext != he) last = last -> PQnext;
		last -> PQnext = he -> PQnext;
		PQcount -= 1;
		deref(he -> vertex);
		he -> vertex = (struct Site *) NULL;
	}
}

int PQbucket(struct Halfedge *he)
{
	int bucket;

	bucket = (he->ystar - ymin)/deltay * PQhashsize;
	if (bucket<0) bucket = 0;
	if (bucket>=PQhashsize) bucket = PQhashsize-1 ;
	if (bucket < PQmin) PQmin = bucket;
	return(bucket);
}

int PQempty()
{
	return(PQcount==0);
}

struct Point PQ_min()
{
	struct Point answer;

	while(PQhash[PQmin].PQnext == (struct Halfedge *)NULL)
	{
		PQmin += 1;
	}
	answer.x = PQhash[PQmin].PQnext -> vertex -> coord.x;
	answer.y = PQhash[PQmin].PQnext -> ystar;
	return (answer);
}

struct Halfedge *PQextractmin()
{
	struct Halfedge *curr;

	curr = PQhash[PQmin].PQnext;
	PQhash[PQmin].PQnext = curr -> PQnext;
	PQcount -= 1;
	return(curr);
}


void PQinitialize()
{
	int i;

	PQcount = 0;
	PQmin = 0;
	PQhashsize = 4 * sqrt_nsites;
	PQhash = (struct Halfedge *) vor_myalloc(PQhashsize * sizeof *PQhash);
	for(i=0; i<PQhashsize; i+=1) PQhash[i].PQnext = (struct Halfedge *)NULL;
}

