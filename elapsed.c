/*
 *  Return elapsed wall time since last call (seconds)
 */
#include "cvglsl.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

static double t0=0;
double Elapsed(void)
{
#ifdef _WIN32
   //  Windows version of wall time
   LARGE_INTEGER tv,freq;
   QueryPerformanceCounter((LARGE_INTEGER*)&tv);
   QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
   double t = tv.QuadPart/(double)freq.QuadPart;
#else
   //  Unix/Linux/OSX version of wall time
   struct timeval tv;
   gettimeofday(&tv,NULL);
   double t = tv.tv_sec+1e-6*tv.tv_usec;
#endif
   double s = t-t0;
   t0 = t;
   return s;
}
