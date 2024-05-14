#include "pgmfiles.h"

/* Read a 8 bit .PGM into a matrix
   allocates memory for the matrix too. */


long int read8bitPGM(eightBitPGMImage *PGMImage)
{
  char c;
  int x,y;
  FILE *filein;

  /* open filein */
  
  if ((filein = fopen(PGMImage->fileName,"r")) == NULL) return (PGMFileOpenError); 
 
  /* read magic value */
 
  if (((c = getc(filein)) != 'P') || ((c = getc(filein)) != '2')) return(PGMFileFormatError);

  while ((c = getc(filein)) != '\n');

  /* read comments */   

  while ((c = getc(filein)) == '#')
    while ((c = getc(filein)) != '\n');
    
  /* read image width */
  
  PGMImage->y = 0;
  do
    if ((c >= '0') && (c <= '9'))
      PGMImage->y = PGMImage->y * 10 + (c - '0');
  while ((c = getc(filein)) != ' ');

	printf("PGMImage->y = %d\n", PGMImage->y);
  /* read image height */

  PGMImage->x = 0;
  do
    if ((c >= '0') && (c <= '9'))
      PGMImage->x = PGMImage->x * 10 + (c - '0');
  while ((c = getc(filein)) != ' ');
    
	printf("PGMImage->x = %d\n", PGMImage->x);
  /* read comments */   
  
  while ((c = getc(filein)) == '#')
    while ((c = getc(filein)) != '\n');
    

  /* read image maxGrey */

 PGMImage->max =0;
  do
    if (c != ' ')
      PGMImage->max = PGMImage->max*10 + (c - '0');
  while ((c = getc(filein)) != '\n');

	printf("PGMImage->max = %d\n", PGMImage->max);

  if (PGMImage->max > 255) return(PGMFileDataIsnt8bit);

  /* alloc space in memory */

  if ((PGMImage->imageData =  (char*) malloc(((PGMImage->x * PGMImage->y) + 1) * sizeof(char) )) == NULL)
      return(PGMMemoryExausted);

  /* read image */

  for (x = 0 ; x < PGMImage->x ; x++)
    for (y = 0 ; y < PGMImage->y ; y++)
	{
		unsigned int ch; 
		fscanf(filein,"%u ",&ch);
	    *(PGMImage->imageData + x*PGMImage->y + y) = ch;
	}

  /* close filein */

  fclose(filein);

  return(PGMImage->y * PGMImage->x);
}

long int write8bitPGM(eightBitPGMImage *PGMImage)
{

  int x,y;
  FILE  *fileout;

  /* open fileout */
     
  if ((fileout = fopen(PGMImage->fileName, "w")) == NULL) return(PGMFileOpenError);

  /* write magic value */
  
  fprintf(fileout, "P5\n");

  /* write width */
  
  fprintf(fileout,"%d\n", PGMImage->y);

  /* write height */
  
  fprintf(fileout,"%d\n", PGMImage->x);

  /* write max value */
  
  fprintf(fileout,"%d\n", PGMImage->max);   

  /* write image */
  for (x = 0 ; x < PGMImage->x ; x++)
    for (y = 0 ; y < PGMImage->y ; y++)
      putc(*(PGMImage->imageData + x*PGMImage->y + y), fileout);
      
  /* close fileout */

  fclose(fileout);
  return(PGMImage->x * PGMImage->y);
}

void printPGMFileError(long int error) 
{
  switch(error) {
  case -1: 
    printf("%s", "Error Opening PGM image file");
    break;
  case -2:
    printf("%s", "Error: not a PGM image file");
    break;
  case -3:
    printf("%s", "The Data in PGM file isn't in 8 bit format");
    break;
  case -4:
    printf("%s", "Error allocating store space for image data");
    break;
  default:
    printf("%s", "Unknow error");
  }
}
