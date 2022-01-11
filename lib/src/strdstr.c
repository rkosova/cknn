#include "../h/strdstr.h"
#include <stdlib.h>
#include <stdio.h>


int count_delim(char *in, char delimiter); 													   		// count number of delimiters in data
int* get_word_size(char *in, int delimiter_count, char delimiter);     								// biggest distance between delimiters
void remove_newline(char *in);																        // remove trailing '\n'

char** strdstr(char *in, char delimiter)
{

	remove_newline(in); 
	int delimiter_count = count_delim(in, delimiter);
	int *word_sizes = get_word_size(in, delimiter_count, ',');

	int prev_word_end = 0, _c=0;
	char **words = malloc(sizeof(char*) * (delimiter_count+1));


	for(int w=0; w<delimiter_count+1; w++) {
		char *temp = malloc(sizeof(char) * (word_sizes[w]+1));
		_c = 0;
		for(int c=prev_word_end; c<=(prev_word_end+word_sizes[w]); c++) {
			if (c<(prev_word_end+word_sizes[w])) {
				*(temp + (c-prev_word_end)) = *(in+c);
				_c = c-prev_word_end;
			} else if (c==(prev_word_end+word_sizes[w])) {
				*(temp + (c-prev_word_end)) = '\0';
				_c = c-prev_word_end;
			}
		}

		prev_word_end = prev_word_end + _c + 1; 													// +1 to account for ','
		*(words+w) = temp;
	}															   

	free(word_sizes);
	return words;				
	
}

int count_delim(char *in, char delimiter)
{
	int offset = 0, count = 0;
	while(*(in+offset)) {
		if(*(in+offset)==delimiter) {
			count++;
		}		

		offset++;
	}

	return count;
}

int* get_word_size(char *in, int delimiter_count, char delimiter)
{

	int *word_sizes = malloc(sizeof(int*) * (delimiter_count+1));
	int offset = 0;
	int word_offset = 0;
	int char_count = 0;

	do {
		if(*(in+offset) != delimiter && *(in+offset) != '\0') {
			char_count++;
		}else{
			*(word_sizes+word_offset) = char_count;
			char_count = 0;
			word_offset++;
		}
		offset++;
	} while(*(in+(offset-1)));																		// -1 to account for offset++

	return word_sizes;
}

void remove_newline(char *in) 
{
	int count=0;

	while(*(in+count)) {
		if (*(in+count) == '\n') {
			*(in+count) = '\0';
		}
		count++;
	}
}

  