/*
* Filename: rpn.c
* Part of: Homework 0
* Author: Ian Polito
* Date Created: 8/27/18
* Date Last Modified: 8/27/18
* Last Modified by: Ian Polito
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Declare Node struct for linked list
struct node {
	int num;
	struct node * next;
};
typedef struct node node;

//Declare Linked List struct
struct llist {
	//pointer to first node in list
	node * head;
	int size;
};
typedef struct llist llist;

//Function prototypes
void push(llist * list, int val);
int pop(llist * list);
void peek(llist * list);
void printList(llist * list);
void clearList(llist * list);
void absList(llist * list);
void dupeList(llist * list);
void unaryList(llist * list);

int main() {
	//create the linked list
	llist * LinkedList;
	LinkedList = (llist*)malloc(sizeof(llist));
	LinkedList->head = NULL;
	LinkedList->size = 0;
	char input[20];
	
	while (1) {
		scanf("%s",&input);
		int val = atoi(input);
		if (val != 0 || (val == 0 && input[0] == '0')) {
			//number, push to stack
			push(LinkedList, val);
		} else if (strcmp(input, "EOF") == 0) {
			//EOF, quit
			break;
		} else if (strcmp(input, "q") == 0) {
			//q, quit
			break;
		} else if (strcmp(input, "+") == 0) {
			//+ pop two digits if possible and push result
			if (LinkedList->size > 1) {
				int a = pop(LinkedList);
				int b = pop(LinkedList);
				push(LinkedList, (a + b));
			} else {
				printf("Stack empty.\n");
			}
		} else if (strcmp(input, "-") == 0) {
			//- pop two digits if possible and push result
			if (LinkedList->size > 1) {
                                int a = pop(LinkedList);
                                int b = pop(LinkedList);
                                push(LinkedList, (a - b));
                        } else {
                                printf("Stack empty.\n");
                        }
		} else if (strcmp(input, "*") == 0) {
			//* pop two digits if possible and push result
			if (LinkedList->size > 1) {
                                int a = pop(LinkedList);
                                int b = pop(LinkedList);
                                push(LinkedList, (a * b));
                        } else {
                                printf("Stack empty.\n");
                        }
		} else if (strcmp(input, "/") == 0) {
			//divide, pop two digits if possible and push result
			if (LinkedList->size > 1) {
                                int a = pop(LinkedList);
                                int b = pop(LinkedList);
				if (b != 0) {
                                	push(LinkedList, (a / b));
				} else {
					printf("Error, divide by zero.\n");
					push(LinkedList, b);
					push(LinkedList, a);
				}
                        } else {
                                printf("Stack empty.\n");
                        }
		} else if (strcmp(input, "p") == 0) {
			//print the top element of stack
			if (LinkedList->size > 0) {
				peek(LinkedList);
			} else {
				printf("Stack empty.\n");
			}
		} else if (strcmp(input, "f") == 0) {
			//print the stack from top to bottom, each element on new line
			printList(LinkedList);
		} else if (strcmp(input, "c") == 0) {
			//clear the stack
			clearList(LinkedList);
		} else if (strcmp(input, "a") == 0) {
			//replace the top stack element with its absolute value
			if (LinkedList->size > 0) {
				absList(LinkedList);
			} else {
				printf("Stack empty.\n");
			}
		} else if (strcmp(input, "m") == 0) {
			//reverse sign of top stack element
			if (LinkedList->size > 0) {
				unaryList(LinkedList);
			} else {
				printf("Stack empty.\n");
			}
		} else if (strcmp(input, "d") == 0) {
			//dupes the top element of stack
			if (LinkedList->size > 0) {
				dupeList(LinkedList);
			} else {
				printf("Stack empty.\n");
			}
		} else {
			//invalid command
			printf("Invalid command.\n");
		}
	}
	clearList(LinkedList);
	free(LinkedList);
	return (0);
}

void push(llist * list, int val) {
	//insert a new node with num equal to val at the start of the
	//given linked list
	node * e = (node*)malloc(sizeof(node));
	e->num = val;
	e->next  = list->head;
	list->head = e;
	list->size = list->size + 1;;
}

int pop(llist * list) {
	//returns the num of the first node in the given linked list
	node * e = list->head;
	if (e == NULL) {
		//empty, so return 0
		return 0;
	} else if (e->next == NULL) {
		//only one node in list, so make head NULL
		list->head = NULL;
		int result = e->num;
		free(e);
		list->size = list->size - 1;
		return result;
	} else {
		//otherwise just remove the first node
		list->head = e->next;
		int result = e->num;
		free(e);
		list->size = list->size - 1;
		return result;
	}
}

void peek(llist * list) {
	//print the first element of linked list
	node * e = list->head;
	printf("%d\n",e->num);
}

void printList(llist * list) {
	//print the list from top to bottom, each element on new line
	node * e = list->head;
	if (e == NULL) {
		printf("Stack empty.\n");
	} else {
		while (e != NULL) {
			printf("%d\n", e->num);
			e = e->next;
		}
	}
}

void clearList(llist * list) {
	//clear the list
	node * e = list->head;
	while (e != NULL) {
		node * n = e;
		e = e->next;
		free(n);
	}
	list->head = NULL;
	list->size = 0;
}

void absList(llist * list) {
	//replace the first list element with its absolute value
	int val = pop(list);
	push(list, abs(val));
}

void dupeList(llist * list) {
	//dupes the first element of the list
	node * e = list->head;
	push(list, e->num);
}

void unaryList(llist * list) {
	//reverse the sign of the first list element
	int val = pop(list);
	if (val < 0) {
		push(list, abs(val));
	} else if (val > 0) {
		val = (-val);
		push(list, val);
	} else {
		push(list, val);
	}
}
