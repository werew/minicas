#ifndef _CMD_H_
#define _CMD_H_

#include "xtypes.h"
#include "ref_all.h"


/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	unsigned int n_args;
	ref_t* types;
	} s_cmd, *Cmd;


/* Create/destroy command */
Cmd new_cmd(Ref (*fun) (ref_list), unsigned int n_args, ref_t* types);
void drop_cmd(Cmd c);
Ref new_cref(char* name,Ref (*fun) (ref_list), unsigned int n_args, ref_t* types);
Ref set_cref(char* name,Ref (*fun) (ref_list), unsigned int n_args, ref_t* types);
Ref get_cmd(const char* name);



void print_cmd(Cmd c);

#endif
