#include "member_array.h"

unsigned int member_array_c(unsigned int * array, int e, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (array[i] == e) {
			//its in the array
			return 1;
		}
	}
	//isnt in array
	return 0;
}
