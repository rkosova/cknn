#include "../h/strdstrtod.h"
#include <stdlib.h>

double* strdstrtod(char **in, int column_number,int class_column)
{
	double *out = malloc(sizeof(double) * column_number);

	if(class_column>column_number) {
		return NULL;
	} else {
		for(int i=0; i<column_number; i++) {
			if(i==class_column) continue;
			*(out+i) = strtod(*(in+i), NULL);
		}

		return out;
	}

	return NULL;
}


