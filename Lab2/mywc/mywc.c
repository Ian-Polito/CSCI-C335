/*
* Filename: mywc.c
* Part of: Lab 2
* Author: Ian Polito
* Date Created: 8/31/18
* Date Last Modified: 8/31/18
* Last Modified by: Ian Polito
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
	int chars = 0;
	int words = 0;
	int lines = 0;
	int c;
	while ((c = getchar()) != EOF) {
		chars++;
		if (c == 32 || c == 13 || c == 9 || c == 11 || c == 12) {
			words++;
		} else if (c == 10) {
			//new line
			words++;
			lines++;
		}
	}
	printf("Total Characters: %d\n", chars);
	printf("Total Words: %d\n", words);
	printf("Total Lines: %d\n", lines);
	return (0);
}
