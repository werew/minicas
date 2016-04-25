
/* Some useful extra types definition */

#ifndef _XTYPES_H_
#define _XTYPES_H_

// Too lazy to use stdbool.h
typedef enum { false, true } bool;


/* Class of characters */
typedef enum { 
	ALPHA, 
	DIGIT, 
	ALPHANUM, 
	SPACE	
	} c_class;


#endif
