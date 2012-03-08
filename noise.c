/*
 * Coherent noise function over 1, 2 or 3 dimensions
 * (copyright Ken Perlin)
 */

#include "cvglsl.h"

const int size = 64;

#define MAXB 0x100
#define N 0x1000

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i, b0, b1, r0, r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at3(rx, ry, rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

static int    p[MAXB + MAXB + 2];
static double g3[MAXB + MAXB + 2][3];

static int start=0;
static int B;
static int BM;

//
//  Normalize 3D vector
//
static void normalize3(double v[3])
{
   double s;

   s = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
   v[0] /=  s;
   v[1] /=  s;
   v[2] /=  s;
}

//
//  Initialize Noise
//
static void InitNoise(void)
{
   int i,j,k;

   srand(30757);
   for (i=0;i<B;i++)
   {
      p[i] = i;
      for (j=0;j<3;j++)
         g3[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i)
   {
      k = p[i];
      p[i] = p[j = rand() % B];
      p[j] = k;
   }

   for (i=0;i<B+2;i++)
   {
      p[B+i] = p[i];
      for (j=0;j<3;j++)
         g3[B+i][j] = g3[i][j];
   }
}

//
//  Set noise frequency
//
static void SetNoiseFrequency(int frequency)
{
   start = 1;
   B = frequency;
   BM = B-1;
}

//
//  3D noise
//
static double noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   if (start)
   {
      start = 0;
      InitNoise();
   }

   setup(0, bx0, bx1, rx0, rx1);
   setup(1, by0, by1, ry0, ry1);
   setup(2, bz0, bz1, rz0, rz1);

   i = p[bx0];
   j = p[bx1];

   b00 = p[i + by0];
   b10 = p[j + by0];
   b01 = p[i + by1];
   b11 = p[j + by1];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[b00 + bz0]; u = at3(rx0, ry0, rz0);
   q = g3[b10 + bz0]; v = at3(rx1, ry0, rz0);
   a = lerp(t,u,v);

   q = g3[b01 + bz0]; u = at3(rx0, ry1, rz0);
   q = g3[b11 + bz0]; v = at3(rx1, ry1, rz0);
   b = lerp(t, u, v);

   c = lerp(sy,a,b);

   q = g3[b00 + bz1]; u = at3(rx0, ry0, rz1);
   q = g3[b10 + bz1]; v = at3(rx1, ry0, rz1);
   a = lerp(t, u, v);

   q = g3[b01 + bz1]; u = at3(rx0, ry1, rz1);
   q = g3[b11 + bz1]; v = at3(rx1, ry1, rz1);
   b = lerp(t, u, v);

   d = lerp(sy,a,b);

   return lerp(sz,c,d);
}

//
//  Create 3D noise texture
//
int CreateNoise3D(int unit)
{
   int f,i,j,k,inc;
   unsigned int id;
   int numOctaves = 4;
   double ni[3];
   double inci,incj,inck;
   int frequency = 4;
   double amp = 0.5;
   char* ptr;
   char pix[size * size * size * 4];

   for (f=0,inc=0; f<numOctaves; f++,frequency*=2,inc++,amp*=0.5)
   {
      SetNoiseFrequency(frequency);
      ptr = pix;
      ni[0] = ni[1] = ni[2] = 0;

      inci = 1.0 / (size / frequency);
      for (i = 0; i < size; ++i, ni[0] += inci)
      {
         incj = 1.0 / (size / frequency);
         for (j = 0; j < size; ++j, ni[1] += incj)
         {
            inck = 1.0 / (size / frequency);
            for (k = 0; k < size; ++k, ni[2] += inck, ptr += 4)
               *(ptr + inc) = (char) (((noise3(ni) + 1.0) * amp) * 128.0);
         }
      }
   }

   //  Select texture unit
   glActiveTexture(unit);

   //  Generate 2D texture id and make current
   glGenTextures(1,&id);
   glBindTexture(GL_TEXTURE_2D,id);

   //  Copy noise to texture
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, size, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);

   //  Set texture parameters
   glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   //  Select texture unit
   glActiveTexture(GL_TEXTURE0);

   return id;
}
