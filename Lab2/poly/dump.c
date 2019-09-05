/*
* Ian Polito
* C335
* Lab 2
* 8/31/18
* Partner: Colin Curry
*/

//
// dump.c
//

void dump_memory(void *p, int len)
{
  int i;
  printf("address      char hexCh short     integer         float    doubleFloat\n");
  for (i = 0; i < len; i++) {
    //print address and char
    if (*(unsigned char *)(p + i) > 31 && *(unsigned char *)(p + i) < 127) {
    	printf("%8p  %c", p + i, *(unsigned char *)(p + i));
    } else {
        printf("%8p  ?", p + i);   
    }
    //print as hexCh
    printf(" 0x%02x", *(unsigned char *)(p + i));
    //print as short
    if (i % 2 == 0) {
    	printf(" %+06hd", *(unsigned int *)(p + i));
    }
    //print as integer
    if (i % 4 == 0) {
        printf(" %+11d", *(unsigned int *)(p + i));
    }
    //print as float
    if (i % 4 == 0) {
        printf(" %+10e", *(float *)(p + i));
    }
    //print as doubleFloat
    if (i % 8 == 0) {
        printf(" %+10e", *(double *)(p + i));
    }
    //print newline
    printf("\n");
  }
}
