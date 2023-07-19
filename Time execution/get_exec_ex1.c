/*  This example prints the time elapsed since the program was invoked.  */
#include <time.h>
#include <stdio.h>

double time1, timedif;   /* use doubles to show small values */

int main(void)
{
    time1 = (double) clock();            /* get initial time */
    time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */

    /* call clock a second time */
    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    printf("The elapsed time is %f seconds\n", timedif);
}