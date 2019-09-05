/*
*Filename: hw3.c
*Part of: Homework 2
*Created by: Ian Polito
*Created on: 10/29/2018
*Last Modified by: Ian Polito
*Last Modified on 10/29/2018
*/

#include <stdio.h>

//function to extract k bits from p position
int extract(int num, int k, int p) {
	return (((1 << k) - 1) & (num >> (p - 1)));
}

int main() {
	//lookup tables
	char * reg[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};
	int inst;
	int type;
	//create file test.out to write asm code to
	FILE * file;
	file = fopen("test.out","w");
	//write first few necessary lines
	fprintf(file,".text\n");
	fprintf(file,".syntax unified\n");
	fprintf(file,".thumb\n");
	//open file test.hex for reading input
	FILE * file2;
	file2 = fopen("test.hex","r");
	//start getting input from test.hex
	while (scanf("%x", &inst) == 1) {
		//check instruction type
		type = inst >> 10;
		if (type == 0) {
			//MOV
			fprintf(file,"\tmovs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
		} else if (type == 1) {
			//LSL
			fprintf(file,"\tlsls r%d,r%d,%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,5,7));
		} else if (type == 2 || type == 3) {
			//LSR
			fprintf(file,"\tlsrs r%d,r%d,%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,5,7));
		} else if (type == 4 || type == 5) {
			//ASR
			fprintf(file,"\tasrs r%d,r%d,%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,5,7));
		} else if (type == 6) {
			//add/subtract register
			//need further checking
			type = inst >> 9;
			if (type == 12) {
				//add
				fprintf(file,"\tadds r%d,r%d,r%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,3,7));
			} else if (type == 13) {
				//sub
				fprintf(file,"\tsubs r%d,r%d,r%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,3,7));
			}
		} else if (type == 7) {
			//add/subtract immediate
			//need further checking
			type = inst >> 9;
			if (type == 14) {
				//add
				fprintf(file,"\tadds r%d,r%d,%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,3,7));
			} else if (type == 15) {
				//sub
				fprintf(file,"\tsubs r%d,r%d,%d\n",extract(inst,3,1),extract(inst,3,4),extract(inst,3,7));
			}
		} else if (type == 8 || type == 9) {
			//MOV immediate
			fprintf(file,"\tmovs r%d,%d\n",extract(inst,3,9),extract(inst,8,1));
		} else if (type == 10 || type == 11) {
			//CMP immediate
			fprintf(file,"\tcmp r%d,%d\n",extract(inst,3,9),extract(inst,8,1));
		} else if (type == 12 || type == 13) {
			//ADD immediate
			fprintf(file,"\tadds r%d,%d\n",extract(inst,3,9),extract(inst,8,1));
		} else if (type == 14 || type == 15) {
			//SUB immediate
			fprintf(file,"\tsubs r%d,%d\n",extract(inst,3,9),extract(inst,8,1));
		} else if (type == 16) {
			//data processing
			//need further checking
			type = extract(inst,4,7);
			if (type == 0) {
				//and
				fprintf(file,"\tands r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 1) {
				//eor
				fprintf(file,"\teors r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 2) {
				//lsl
				fprintf(file,"\tlsls r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 3) {
				//lsr
				fprintf(file,"\tlsrs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 4) {
				//asr
				fprintf(file,"\tasrs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 5) {
				//adc
				fprintf(file,"\tadcs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 6) {
				//sbc
				fprintf(file,"\tsbcs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 7) {
				//ror
				fprintf(file,"\trors r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 8) {
				//tst
				fprintf(file,"\ttst r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 9) {
				//rsb
				fprintf(file,"\trsbs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 10) {
				//cmp
				fprintf(file,"\tcmp r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 11) {
				//cmn
				fprintf(file,"\tcmn r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 12) {
				//orr
				fprintf(file,"\torrs r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 13) {
				//mul
				fprintf(file,"\tmuls r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 14) {
				//bic
				fprintf(file,"\tbics r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			} else if (type == 15) {
				//mvn
				fprintf(file,"\tmvns r%d,r%d\n",extract(inst,3,1),extract(inst,3,4));
			}
		} else if (type == 17) {
			//special data processing
			//need further checking
			type = extract(inst,2,9);
                        if (type == 0) {
				//add
				fprintf(file,"\tadds r%d,%s\n",extract(inst,3,1),reg[extract(inst,4,4)]);
			} else if (type == 1) {
				//cmp
				fprintf(file,"\tcmp r%d,%s\n",extract(inst,3,1),reg[extract(inst,4,4)]);
			} else if (type == 2) {
				//mov
				fprintf(file,"\tmovs r%d,%s\n",extract(inst,3,1),reg[extract(inst,4,4)]);
			}
		} else {
			//undefined instruction
			fprintf(file,"\t.hword 0x%x\n",inst);
		}
	}
	fclose(file);
	fclose(file2);
	return(0);
}
