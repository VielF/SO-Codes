#include <stdio.h>
#include <stdlib.h>

unsigned char *image; // ,*imageA,*imageB;
int  width,height,max;


/* transform 2D to 1D */

int point(int x,int y)
{
  int p;
  /* set p to apoint a special point */  
  
  p = width*height; 
  
  /* set de value of special point to 0 */
  
  image[width*height]=0;

  /* if x and y apoint to a valid point calculate the value for p */  
  
  if (x < width)
    if  (y < height)
      if (x >= 0)
	if (y >= 0) 
	  p = (y * width) + x;
 
  /* return the value of p */
  return(p);
}

void readpgm(char *file)
{
  char c;
  int x,y;
  FILE *filein;

  /* open filein */
  
  if ((filein = fopen(file,"r")) == NULL)
    {
      fprintf(stderr,"\nmain:   cannot open %s\n",file);
      exit(1);
    }
 
  /* read magic value */
 
  if (((c = getc(filein)) != 'P') || ((c = getc(filein)) != '5'))
    {
      fprintf(stderr,"\nmain:   infile not is PGM\n");
      exit(1);
    }
  while ((c = getc(filein)) != '\n');

  /* read comments */   

  while ((c = getc(filein)) == '#')
    while ((c = getc(filein)) != '\n');
    
  /* read image width */
  
  width = 0;
  do
    if ((c >= '0') && (c <= '9'))
      width = width*10 + (c - '0');
  while ((c = getc(filein)) != ' ');
  
  /* read image height */

  height = 0;
  do
    if ((c >= '0') && (c <= '9'))
      height = height*10 + (c - '0');
  while ((c = getc(filein)) != '\n');
    
  /* read comments */   
  
  while ((c = getc(filein)) == '#')
    while ((c = getc(filein)) != '\n');
    

  /* read image maxGrey */

  max =0;
  do
    if (c != ' ')
      max = max*10 + (c - '0');
  while ((c = getc(filein)) != '\n');
  if (max > 255) 
    {    
     fprintf(stderr,"\nonly 8 bits images\n");
     exit(1);
    }
  
  /* alloc space in memory */

  if ((image =  (char*) malloc(((width * height) + 1) * sizeof(char) )) == NULL)
    {
      fprintf(stderr,"\ndo not have memory\n");
      exit(1);
    }
  /* read image */

  for (y = 0 ; y < height ; y++)
    for (x = 0 ; x < width ; x++)
      image[point(x,y)] = getc(filein);
  

  /* close filein */

  fclose(filein);

}

void writelist(char *file)
{

int x,y;
FILE  *fileout;

  /* open fileout */
     
  if ((fileout = fopen(file,"w")) == NULL)
    {
      fprintf(stderr,"\nmain:   cannot open %s\n",file);
      exit(1);
    }    
  /* write image */
  for (y = 0 ; y < height ; y++) {
    for (x = 0 ; x < width ; x++) {
      fprintf(fileout, "%d ", image[point(x,y)]);
    }
    fprintf(fileout, "\n");
  }
      
  /* close fileout */

  fclose(fileout);

  /* disalloc memory */ 

  free(image);
}

/* Main function */

/*int main(argc,argv)
     int argc;
     char *argv[];
{
  

  if (argc != 3)
    {
      fprintf(stderr,"\nusage: %s infile outfile\n", argv[0]);
      exit(1);
    }

  printf("\nreading image"); 
  readpgm (argv[1]);
  

//  printf("\napplying matched filter");
//  match();
//  printf("\napplying bix rim filter");
//     boxrim(); 
    
  printf("\nwriting Results\n");
  writelist (argv[2]);
  
  return 0;    
}*/




