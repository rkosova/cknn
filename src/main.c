#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/h/strdstr.h"
#include "../h/strdstrtod.h"
#include "../h/distance.h"

// comments are temporary docs so i dont forget what code does when i write actual docs lol

typedef struct {
	int index;
	double distance;
	char const *class_value;
} ClassInfo;  


int get_rows(FILE *classified_data, int const BUFFER_SIZE);
void temp_sort(ClassInfo *data, int num_of_rows);
void k_classify(ClassInfo *data, int k);
char is_class_in(char const *word, char const **data, int *added);
void k_classify(ClassInfo *data, int k);


int main(int argc, char const *argv[])
{

	int const BUFFER_SIZE = 700;

	char const *path_to_classified_data = argv[1];;
	char const *path_to_unclassified_data = argv[2];
	int class_column = strtol(argv[3], NULL, 0) - 1;


	FILE *classified_data = fopen(path_to_classified_data, "r");
	FILE *unclassified_data = fopen(path_to_unclassified_data, "r");

	char unclassified_buffer[BUFFER_SIZE];
	int num_of_columns;
	int num_of_rows;


	while(fgets(unclassified_buffer, BUFFER_SIZE, unclassified_data)) {
		char **unclassified_separate = strdstr(unclassified_buffer, ',');

		num_of_columns = count_delim(unclassified_buffer, ',') + 1;
		num_of_rows = get_rows(classified_data, BUFFER_SIZE);
		
		double *unclassified_converted = strdstrtod(unclassified_separate, num_of_columns, -1);

		char classified_buffer[BUFFER_SIZE];
		char **classified_separate;
		double *classified_converted;
		 		
		ClassInfo distances[num_of_rows];

		int i = 0;

		while(fgets(classified_buffer, BUFFER_SIZE, classified_data)) {
			classified_separate = strdstr(classified_buffer, ',');
			classified_converted = strdstrtod(classified_separate, num_of_columns, class_column);

			distances[i].index = i;
			distances[i].distance = distance(unclassified_converted, classified_converted, num_of_columns);
			distances[i].class_value = classified_separate[class_column];

			i++;
		}	

		temp_sort(distances, num_of_rows);


		k_classify(distances, 3);

		// printf("\n==========================================================\n");


		// for(int i=0; i<num_of_rows; i++) {
		// 	printf("\nindex: %i, distance: %lf, class: %s\n", distances[i].index, distances[i].distance, distances[i].class_value);
		// }

		// printf("Data point %s has class %s\n", unclassified_buffer, distances[0].class_value);


		// printf("\n==========================================================\n");

		rewind(classified_data);

		

		// free memory


	}

	return 0;
}


int get_rows(FILE *data, int const BUFFER_SIZE) 
{
	char buffer[BUFFER_SIZE];
	int rows = 0;
	while(fgets(buffer, BUFFER_SIZE, data)) {
		rows++;
	}
	rewind(data);
	return rows;
} 


void temp_sort(ClassInfo *data, int num_of_rows) // selection sort for now
{
	ClassInfo temp;
	int pos_current_smallest = 0;
	for(int i=0; i<num_of_rows; i++) {
		temp = *(data+i);
		for(int j=i+1; j<num_of_rows; j++) {
			if (temp.distance > (data+j)->distance) {
				temp = *(data+j);
				pos_current_smallest = j;
				*(data+pos_current_smallest) = *(data+i);
				*(data+i) = temp;
			}
		}

		// printf("\niter %i, i: %i, pos_current_smallest: %i\n", i, i, pos_current_smallest);

		// printf("--------------------------------------------------\n");

		// for(int i=0; i<num_of_rows; i++) {
		// 	printf("\nindex: %i, distance: %lf, class: %s\n", data[i].index, data[i].distance, data[i].class_value);
		// }

		// printf("--------------------------------------------------\n");
	}
}

// TODO: 

//  - Add check to see if number of rows > k
// 	- Classify using k-neighbours

char is_class_in(char const *class, char const **classes_u, int *added)
{
	for (int i=0; i < *added; i++) {
		if (!strcmp(class, *(classes_u+i))) {
			return 1;
		}
	}

	(*added)++;
	return 0;
}



void k_classify(ClassInfo *data, int k)
{
	char const *classes_unique[k];
	int added = 0;

	classes_unique[0] = data->class_value;

	for (int i=1; i<k; i++) {
		if (!is_class_in((data+i)->class_value, classes_unique, &added)) {
			classes_unique[added] = (data+i)->class_value;
		}
	}

	printf("%i\n", added); 

	for (int i=0; i<added; i++) {
		printf("%c\n", *classes_unique[i]);
	} // works up to here!! gets all unique classes
}


