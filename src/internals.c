#include "ref_all.h"
#include "internals.h"
#include "error.h"
#include <stdlib.h>


void load_commands(void){
	Ref load = NULL;
	
	load = set_cref("quit",quit_cmd,0,NULL);
	if (load == NULL) inst_err(ELOAD,"command quit");

	

}


#pragma GCC diagnostic ignored "-Wunused-parameter"
Ref quit_cmd(ref_list args){ // Respect general prototype
	exit(0);
}
#pragma GCC diagnostic pop
