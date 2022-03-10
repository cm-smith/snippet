/* ========================================================================== */
/* File: common.c
 *
 * Project name: Snippet
 *
 * Author: Michael Smith
 * Date: 2/1/2020
 *
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>			// strlen
#include <stdlib.h>			// malloc
#include <stdio.h>			// printf (debug)

// ---------------- Local includes  e.g., "file.h"
#include "utils.h"                      // utils functionality

// ---------------- Constant definitions

// ---------------- Macro definitions
#define SAFEFREE(a) free(a); a=NULL;

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

// https://stackoverflow.com/a/16871702
// The size is extended by the input with the value of the provisional
char* inputString(FILE* fp, size_t size){
	char *str;
	int ch;
	size_t len = 0;

	// Size is start size
	str = realloc(NULL, sizeof(char)*size);
	
	if(!str) return str;

	while(EOF != (ch = fgetc(fp)) && ch != '\n') {
	    str[len++]=ch;
	    if(len==size) {
	        str = realloc(str, sizeof(char)*(size+=16));
	        if(!str)return str;
	    }
	}

	str[len++] = '\0';
	
	return realloc(str, sizeof(char)*len);
}

