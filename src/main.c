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
char is_class_in(char const *word, char const **data, int added);
void k_classify(ClassInfo *data, int k, FILE *classifying_file);


int main(int argc, char const *argv[])
{

	int const BUFFER_SIZE = 700;

	char const *path_to_classified_data = argv[1];
	char const *path_to_unclassified_data = argv[2];
	char const *path_to_classifying_data = argv[3];
	int class_column = strtol(argv[4], NULL, 0) - 1;
	int k = strtol(argv[5], NULL, 0);

	FILE *classified_data = fopen(path_to_classified_data, "r");
	FILE *unclassified_data = fopen(path_to_unclassified_data, "r");
	FILE *classifying_data = fopen(path_to_classifying_data, "w");

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
			free(classified_separate);
			free(classified_converted);
		}	

		temp_sort(distances, num_of_rows);
		k_classify(distances, k, classifying_data);
		rewind(classified_data);
		free(unclassified_separate);
		free(unclassified_converted);
	}

	
	fclose(classified_data);
	fclose(unclassified_data);
	fclose(classifying_data);
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
	}
}


char is_class_in(char const *class, char const **classes_u, int added)
{
	for (int i=0; i < added; i++) {
		if (!strcmp(class, *(classes_u+i))) {
			return 1;
		}
	}

	return 0;
}


void k_classify(ClassInfo *data, int k, FILE *classifying_file)
{
	char const *classes_unique[k];
	int added = 0;

	classes_unique[0] = data->class_value;
	added = 1;

	for (int i=1; i<k; i++) {
		if (!is_class_in((data+i)->class_value, classes_unique, added)) {
			classes_unique[added] = (data+i)->class_value;
			added++;
		}
	}

	double avg_distances[added];
	double count = 0.0;


	for (int i=0; i<added; i++) {
		count = 0.0;
		for (int j=0; j<k; j++) {
			if (!strcmp((data+j)->class_value, *(classes_unique+i))) {
				count++;
				avg_distances[i] += (data+j)->distance;
			}
		}

		avg_distances[i] /= count;
	}


	int smallest_index = 0;

	for (int i=smallest_index; i<added; i++) {
		for (int j=i; j<added; j++) {
			if (avg_distances[i] > avg_distances[j]) {
				smallest_index = j;
			}
		}
	}

	// LEFT TO DO:
	// - Free memory,
	// - Clean up code and remove debug info.

	fprintf(classifying_file, "%s\n", classes_unique[smallest_index]);
}
