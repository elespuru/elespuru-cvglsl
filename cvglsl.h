#ifndef CVGLSL
#define CVGLSL

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void         Print(const char* format , ...);
void         Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void         Project(double fov,double asp,double dim);
void         ErrCheck(const char* where);
int          LoadOBJ(const char* file);
int          FramesPerSecond(void);
double       Elapsed(void);
int          CreateShaderProg(char* VertFile,char* FragFile);
int          CreateShaderProgLoc(char* VertFile,char* FragFile,char* Name[]);
int          CreateShaderProgGeom(char* VertFile,char* GeomFile,char* FragFile,int in,int out,int n);
int          CreateNoise3D(int unit);

#ifdef __cplusplus
}
#endif

#endif

