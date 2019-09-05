/* Filename: hello.c
*  Part of: C335 Lab 1
*  Created by: Ian Polito
*  Created on: 8/24/2018
*  Last Modified by: Ian Polito
*  Last Modified on 8/24/2018
*/

#include <stdio.h>
#include <unistd.h> 

int main() {
	printf("Hello World\n");
	while (1) {
		printf("\n");
		sleep(3);
	}
	return(0);
}
