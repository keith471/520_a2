#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void addHeaderCode(FILE* emitFILE) {
   FILE* headerFILE;
   char c;

   headerFILE = fopen("headercode.txt", "r");

   while ((c = fgetc(headerFILE)) != EOF) {
       fputc(c, emitFILE);
   }

   fclose(headerFILE);
}

void genPROGRAM(PROGRAM* program, char* fname) {
    FILE* emitFILE = fopen(fname, "w");
    addHeaderCode(emitFILE);

    fclose(emitFILE);
}



/*

int main(void)
{
	char* mini_awesome = "";

	mini_awesome = "this is cool!";
	printf("%s", mini_awesome);

	return 0;
}
*/
