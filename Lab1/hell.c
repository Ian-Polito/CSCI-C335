/* Filename: hell.c
*  Part of: C335 Lab 1
*  Created by: Ian Polito
*  Created on: 8/24/2018
*  Last Modified by: Ian Polito
*  Last Modified on 8/24/2018
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
	printf("Hello World\n");
	int i = 0;
	char filename[15];
	while (1) {
		//make new file name
		strcpy(filename, "file");
		char num[5];
		sprintf(num, "%d", i);
		strcat(filename, num);
		strcat(filename, ".txt");
		//create an empty file
		FILE * fp;
		fp = fopen(filename, "w");
		fclose(fp);
		//print new line
		printf("\n");
		sleep(3);
		i++;
	}
	return(0);
}
