#include "support.h"
#include <stdio.h>
#include <string.h>

/* Manipulate the path to lead com name, extensions and special characters */
char* padding(char *filename){
    char output[11];
    char* strptr = filename;
    char* dot;
    dot = strchr(filename, '.');

    int i;
    for(i=0; strptr != dot; strptr++, i++){
    	if(i==8)
    		break;
    	output[i] = *strptr;
    }

    int trail = 8 - i;
    for(; trail > 0; trail--, i++){
    	output[i] = ' ';
    }

    strptr = dot;
    strptr++;
    for(i=8; i < 11; strptr++, i++){
    	output[i] = *strptr;
    }

    output[11] = '\0';
    for(i = 0; output[i] != '\0'; i++){
    	output[i] = toupper(output[i]);
    }
    
    char* out = output;
    return out;

}