#include "cvglsl.h"

//
//  Read text file
//
static char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n+1,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

//
//  Print Shader Log
//
static void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

//
//  Print Program Log
//
static void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

//
//  Create Shader
//
static int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

//
//  Create Shader Program
//
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if (VertFile)
   {
      int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
      glAttachShader(prog,vert);
   }
   //  Create and compile fragment shader
   if (FragFile)
   {
      int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
      glAttachShader(prog,frag);
   }
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

//
//  Create Shader Program including Geometry Shader
//
int CreateShaderProgGeom(char* VertFile,char* GeomFile,char* FragFile,int in,int out,int n)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if (VertFile)
   {
      int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
      glAttachShader(prog,vert);
   }
   //  Create and compile geometry shader
   //  Also set geometry parameters
   if (GeomFile)
   {
#ifdef GL_GEOMETRY_SHADER_EXT
      int geom = CreateShader(GL_GEOMETRY_SHADER_EXT,GeomFile);
      glAttachShader(prog,geom);
      glProgramParameteriEXT(prog,GL_GEOMETRY_INPUT_TYPE_EXT,in);
      glProgramParameteriEXT(prog,GL_GEOMETRY_OUTPUT_TYPE_EXT,out);
      glProgramParameteriEXT(prog,GL_GEOMETRY_VERTICES_OUT_EXT,n);
#else
      Fatal("Geometry shaders not supported\n");
#endif
   }
   //  Create and compile fragment shader
   if (FragFile)
   {
      int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
      glAttachShader(prog,frag);
   }
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

//
//  Create Shader Program with Location Names
//
int CreateShaderProgLoc(char* VertFile,char* FragFile,char* Name[])
{
   int k;
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if (VertFile)
   {
      int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
      glAttachShader(prog,vert);
   }
   //  Create and compile fragment shader
   if (FragFile)
   {
      int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
      glAttachShader(prog,frag);
   }
   //  Set names
   for (k=0;Name[k];k++)
      if (Name[k][0])
         glBindAttribLocation(prog,k,Name[k]);
   ErrCheck("CreateShaderProg");
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}
