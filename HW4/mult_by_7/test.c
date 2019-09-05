#include <stdio.h>
#include "mult_by_7_c.h"

void mult_by_7_test() {
	//test -4
	if (mult_by_7(-4) != mult_by_7_c(-4)) {
		printf("mult_by_7 test 1 failed!\n");
		printf("Input: -4, Expected: %d, Returned: %d\n", mult_by_7_c(-4), mult_by_7(-4));
	} else {
		printf("mult_by_7 test 1 passed!\n");
	}
	//test 0
	if (mult_by_7(0) != mult_by_7_c(0)) {
                printf("mult_by_7 test 2 failed!\n");
                printf("Input: 0, Expected: 0, Returned: %d\n", mult_by_7(0));
        } else {
                printf("mult_by_7 test 2 passed!\n");
        }

	//test 7
	if (mult_by_7(7) != mult_by_7_c(7)) {
                printf("mult_by_7 test 1 failed!\n");
                printf("Input: 7, Expected: %d, Returned: %d\n", mult_by_7_c(7), mult_by_7(7));
        } else {
                printf("mult_by_7 test 3 passed!\n");
        }
}
