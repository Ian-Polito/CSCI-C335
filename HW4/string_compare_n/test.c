#include <stdio.h>
#include <string.h>
#include "string_compare_n_c.h"

void strcmp_test(void) {
	//test "and and"
	if (string_compare_n("and", "and", 3) != string_compare_n_c("and", "and", 3)) {
		printf("string_n_compare test 1 failed!\n");
		printf("str1: and, str2: and, num: 3, Expected: %d, Returned: %d", string_compare_n_c("and", "and", 3), string_compare_n("and","and",3));
	} else {
		printf("string_n_compare test 1 passed!\n");
	}
	
	if (string_compare_n("high", "low", 3) != string_compare_n_c("high", "low", 3)) {
                printf("string_n_compare test 2 failed!\n");
                printf("str1: high, str2: low, num: 3, Expected: %d, Returned: %d", string_compare_n_c("high", "low", 3), string_compare_n("high","low",3));
        } else {
                printf("string_n_compare test 2 passed!\n");
        }

	if (string_compare_n("over", "here", 4) != string_compare_n_c("over", "here", 4)) {
                printf("string_n_compare test 3 failed!\n");
                printf("str1: over, str2: here, num: 4, Expected: %d, Returned: %d", string_compare_n_c("over", "here", 4), string_compare_n("over","here",4));
        } else {
                printf("string_n_compare test 3 passed!\n");
        }

}
