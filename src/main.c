#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "parser.h"
#include "init.h"

//int main(int argc, char **argv) { //TODO add -f option
int main(void){
   struct stat buf;
   
   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0;

   init_env();

   if (!fstat(0, &buf) && S_ISREG(buf.st_mode)) {

       line=NULL;
       while (getline(&line, &n, f_in) != -1) {
	   eval_input(line);
           free(line); line=NULL;
        }

    } else {

       printf("minic@s> ");
       line=NULL;
       while (getline(&line, &n, f_in) != -1) {
	   eval_input(line);
           printf("minic@s> ");
           free(line); line=NULL;
        }
    }
       
   return 0;   
}
