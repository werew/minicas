#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "parser.h"
#include "internals.h"

//int main(int argc, char **argv) { //TODO add -f option
int main(void){
   struct stat buf;
   
   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0;

   if (!fstat(0, &buf) && S_ISREG(buf.st_mode)) {

       line=NULL;
       while (getline(&line, &n, f_in) != -1) {
	   eval_input(line);
           free(line); line=NULL;
        }

    } else {

       printf("prompt> ");
       line=NULL;
       while (getline(&line, &n, f_in) != -1) {
	   eval_input(line);
           printf("prompt> ");
           free(line); line=NULL;
        }
    }
       
   return 0;   
}
