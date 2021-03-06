#ifndef _ERROR_H_
#define _ERROR_H

typedef enum { 
	SUCCESS,
	ESYNTAX, 
	ENOTAFUN,
	ENOTACMD,
	ENOTAVAR,
	ETYPE,
	ELOAD,
	ENOVAL,
	EMXDIM,
	EMISSARG
	} Error;

#define MAX_EMSG 100
extern char err_arg[MAX_EMSG];
extern Error local_err;

void set_err(Error e, char* arg);
void print_error(void);
void inst_err(Error e, char* arg);


#endif
