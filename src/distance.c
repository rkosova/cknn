#include "../h/distance.h"
#include <math.h>

double distance(double *unclassified_vector, double *classified_vector, int dimension) 
{
	double distance = 0;

	for(int i=0; i<dimension; i++) {
		distance += (*(unclassified_vector+i) - *(classified_vector+i)) * (*(unclassified_vector+i) - *(classified_vector+i));
	}

	return sqrt(distance);
}
