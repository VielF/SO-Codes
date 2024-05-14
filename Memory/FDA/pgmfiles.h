#include <stdio.h>
#include <stdlib.h>

#ifndef PGM_FILES
#define PGM_FILES

#define PGMFileOpenError -1     /* Error Opening PGM image file */
#define PGMFileFormatError -2   /* Error: not a PGM image file */
#define PGMFileDataIsnt8bit -3  /* The Data in PGM file isn't in 8 bit format */
#define PGMMemoryExausted -4    /* Error allocating RAM for storing image data */

/* The data type defined below is for manipulating PGM image files data 
   into programs that use these routines */

typedef struct eightBitPGMImageStruct {
  char fileName[255];  /* Image File Name */
  unsigned char  *imageData;        /* Data readed from / to write into Image File Name */
  int x,               /* Image width */
    y,                 /* Image height */
    max;               /* Image Max Gray */
} eightBitPGMImage;
  
long int read8bitPGM(eightBitPGMImage *PGMImage);  /* read a PGM Image according to the struct PGMImage */
long int write8bitPGM(eightBitPGMImage *PGMImage); /* write a PGM Image according to the struct PGMImage */
void printPGMFileError(long int error);        /* print a message corresponding to the error code */

#endif
