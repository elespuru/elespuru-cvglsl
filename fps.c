/*
 *  Frames per seconds
 */
#include "cvglsl.h"

static int fps=0,sec0=0,count=0;
int FramesPerSecond(void)
{
   int sec = glutGet(GLUT_ELAPSED_TIME)/1000;
   if (sec!=sec0)
   {
      sec0 = sec;
      fps = count;
      count=0;
   }
   count++;
   return fps;
}
