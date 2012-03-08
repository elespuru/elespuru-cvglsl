#include "cvglsl.h"
#include <ctype.h>

//  Load an OBJ file
//  Vertex, Normal and Texture coordinates are supported
//  Materials are supported
//  Textures must be BMP files
//  Surfaces are not supported
//
//  WARNING:  This is a minimalist implementation of the OBJ file loader.  It
//  will only correctly load a small subset of possible OBJ files.  It is
//  intended to be a starting point to allow you to load models, but in order
//  for you to do this you will have to understand the OBJ format and most
//  likely will have to significantly enhance this code.
//
//  WARNING:  There are lots of really broken OBJ files on the internet.  Some
//  files may have correct surfaces, but the normals are complete junk and so
//  the lighting is totally broken.  So beware of which OBJ files you use.

//  Material structure
typedef struct
{
   char* name;                 //  Material name
   float Ka[4],Kd[4],Ks[4],Ns; //  Colors and shininess
   float d;                    //  Transparency
   int map;                    //  Texture
} mtl_t;

//  Material count and array
static int Nmtl=0;
static mtl_t* mtl=NULL;

//
//  Return true if CR or LF
//
static int CRLF(char ch)
{
   return ch == '\r' || ch == '\n';
}

//
//  Read line from file
//    Returns pointer to line or NULL on EOF
//
static int linelen=0;    //  Length of line
static char* line=NULL;  //  Internal storage for line
static char* readline(FILE* f)
{
   char ch;  //  Character read
   int k=0;  //  Character count
   while ((ch = fgetc(f)) != EOF)
   {
      //  Allocate more memory for long strings
      if (k>=linelen)
      {
         linelen += 8192;
         line = (char*)realloc(line,linelen);
         if (!line) Fatal("Out of memory in readline\n");
      }
      //  End of Line
      if (CRLF(ch))
      {
         // Eat extra CR or LF characters (if any)
         while ((ch = fgetc(f)) != EOF)
           if (!CRLF(ch)) break;
         //  Stick back the overrun
         if (ch != EOF) ungetc(ch,f);
         //  Bail
         break;
      }
      //  Pad character to line
      else
         line[k++] = ch;
   }
   //  Terminate line if anything was read
   if (k>0) line[k] = 0;
   //  Return pointer to line or NULL on EOF
   return k>0 ? line : NULL;
}

//
//  Read to next non-whitespace word
//  Note that this destroys line in the process
//
static char* getword(char** line)
{
   char* word;
   //  Skip leading whitespace
   while (**line && isspace(**line))
      (*line)++;
   if (!**line) return NULL;
   //  Start of word
   word = *line;
   //  Read until next whitespace
   while (**line && !isspace(**line))
      (*line)++;
   //  Mark end of word if not NULL
   if (**line)
   {
      **line = 0;
      (*line)++;
   }
   return word;
}

//
//  Read n floats
//
static void readfloat(char* line,int n,float x[])
{
   int i;
   for (i=0;i<n;i++)
   {
      char* str = getword(&line);
      if (!str)  Fatal("Premature EOL reading %d floats\n",n);
      if (sscanf(str,"%f",x+i)!=1) Fatal("Error reading float %d\n",i);
   }
}

//
//  Read coordinates
//    n is how many coordiantes to read
//    N is the coordinate index
//    M is the number of coordinates
//    x is the array
//    This function adds more memory as needed in 8192 work chunks
//
static void readcoord(char* line,int n,float* x[],int* N,int* M)
{
   //  Allocate memory if necessary
   if (*N+n > *M)
   {
      *M += 8192;
      *x = (float*)realloc(*x,(*M)*sizeof(float));
      if (!*x) Fatal("Cannot allocate memory\n");
   }
   //  Read n coordinates
   readfloat(line,n,(*x)+*N);
   (*N)+=n;
}

//
//  Read string conditionally
//     Line must start with skip string
//     After skip sting return first word
//     getword terminates the line
//
static char* readstr(char* line,const char* skip)
{
   //  Check for a match on the skip string
   while (*skip && *line && *skip==*line)
   {
      skip++;
      line++;
   }
   //  Skip must be NULL for a match
   if (*skip || !isspace(*line)) return NULL;
   //  Read string
   return getword(&line);
}

//
//  Load materials from file
//
static void LoadMaterial(const char* file)
{
   int k=-1;
   char* line;
   char* str;

   //  Open file or return with warning on error
   FILE* f = fopen(file,"r");
   if (!f)
   {
      fprintf(stderr,"Cannot open material file %s\n",file);
      return;
   }

   //  Read lines
   while ((line = readline(f)))
   {
      //  New material
      if ((str = readstr(line,"newmtl")))
      {
         int l = strlen(str);
         //  Allocate memory for structure
         k = Nmtl++;
         mtl = (mtl_t*)realloc(mtl,Nmtl*sizeof(mtl_t));
         //  Store name
         mtl[k].name = (char*)malloc(l+1);
         if (!mtl[k].name) Fatal("Cannot allocate %d for name\n",l+1);
         strcpy(mtl[k].name,str);
         //  Initialize materials
         mtl[k].Ka[0] = mtl[k].Ka[1] = mtl[k].Ka[2] = 0;   mtl[k].Ka[3] = 1;
         mtl[k].Kd[0] = mtl[k].Kd[1] = mtl[k].Kd[2] = 0;   mtl[k].Kd[3] = 1;
         mtl[k].Ks[0] = mtl[k].Ks[1] = mtl[k].Ks[2] = 0;   mtl[k].Ks[3] = 1;
         mtl[k].Ns  = 0;
         mtl[k].d   = 0;
         mtl[k].map = 0;
      }
      //  If no material short circuit here
      else if (k<0)
      {}
      //  Ambient color
      else if (line[0]=='K' && line[1]=='a')
         readfloat(line+2,3,mtl[k].Ka);
      //  Diffuse color
      else if (line[0]=='K' && line[1] == 'd')
         readfloat(line+2,3,mtl[k].Kd);
      //  Specular color
      else if (line[0]=='K' && line[1] == 's')
         readfloat(line+2,3,mtl[k].Ks);
      //  Material Shininess
      else if (line[0]=='N' && line[1]=='s')
         readfloat(line+2,1,&mtl[k].Ns);
      //  Textures (must be BMP - will fail if not)
      else if ((str = readstr(line,"map_Kd")))
         mtl[k].map = LoadTexBMP(str);
      //  Ignore line if we get here
   }
   fclose(f);
}

//
//  Set material
//
static void SetMaterial(const char* name)
{
   int k;
   //  Search materials for a matching name
   for (k=0;k<Nmtl;k++)
      if (!strcmp(mtl[k].name,name))
      {
         //  Set material colors
         glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,mtl[k].Ka);
         glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,mtl[k].Kd);
         glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,mtl[k].Ks);
         glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&mtl[k].Ns);
         //  Bind texture if specified
         if (mtl[k].map)
         {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,mtl[k].map);
         }
         else
            glDisable(GL_TEXTURE_2D);
         return;
      }
   //  No matches
   fprintf(stderr,"Unknown material %s\n",name);
}

//
//  Load OBJ file
//
int LoadOBJ(const char* file)
{
   int k;
   int  Nv,Nn,Nt;  //  Number of vertex, normal and textures
   int  Mv,Mn,Mt;  //  Maximum vertex, normal and textures
   float* V;       //  Array of vertexes
   float* N;       //  Array of normals
   float* T;       //  Array if textures coordinates
   char*  line;    //  Line pointer
   char*  str;     //  String pointer

   //  Open file
   FILE* f = fopen(file,"r");
   if (!f) Fatal("Cannot open file %s\n",file);

   // Reset materials
   mtl = NULL;
   Nmtl = 0;

   //  Start new displaylist
   int list = glGenLists(1);
   glNewList(list,GL_COMPILE);
   //  Push attributes for textures
   glPushAttrib(GL_TEXTURE_BIT);

   //  Read vertexes and facets
   V  = N  = T  = NULL;
   Nv = Nn = Nt = 0;
   Mv = Mn = Mt = 0;
   while ((line = readline(f)))
   {
      //  Vertex coordinates (always 3)
      if (line[0]=='v' && line[1]==' ')
         readcoord(line+2,3,&V,&Nv,&Mv);
      //  Normal coordinates (always 3)
      else if (line[0]=='v' && line[1] == 'n')
         readcoord(line+2,3,&N,&Nn,&Mn);
      //  Texture coordinates (always 2)
      else if (line[0]=='v' && line[1] == 't')
         readcoord(line+2,2,&T,&Nt,&Mt);
      //  Read and draw facets
      else if (line[0]=='f')
      {
         line++;
         //  Read Vertex/Texture/Normal triplets
         glBegin(GL_POLYGON);
         while ((str = getword(&line)))
         {
            int Kv,Kt,Kn;
            //  Try Vertex/Texture/Normal triplet
            if (sscanf(str,"%d/%d/%d",&Kv,&Kt,&Kn)==3)
            {
               if (Kv<0 || Kv>Nv/3) Fatal("Vertex %d out of range 1-%d\n",Kv,Nv/3);
               if (Kn<0 || Kn>Nn/3) Fatal("Normal %d out of range 1-%d\n",Kn,Nn/3);
               if (Kt<0 || Kt>Nt/2) Fatal("Texture %d out of range 1-%d\n",Kt,Nt/2);
            }
            //  Try Vertex//Normal pairs
            else if (sscanf(str,"%d//%d",&Kv,&Kn)==2)
            {
               if (Kv<0 || Kv>Nv/3) Fatal("Vertex %d out of range 1-%d\n",Kv,Nv/3);
               if (Kn<0 || Kn>Nn/3) Fatal("Normal %d out of range 1-%d\n",Kn,Nn/3);
               Kt = 0;
            }
            //  Try Vertex index
            else if (sscanf(str,"%d",&Kv)==1)
            {
               if (Kv<0 || Kv>Nv/3) Fatal("Vertex %d out of range 1-%d\n",Kv,Nv/3);
               Kn = 0;
               Kt = 0;
            }
            //  This is an error
            else
               Fatal("Invalid facet %s\n",str);
            //  Draw vectors
            if (Kt) glTexCoord2fv(T+2*(Kt-1));
            if (Kn) glNormal3fv(N+3*(Kn-1));
            if (Kv) glVertex3fv(V+3*(Kv-1));
         }
         glEnd();
      }
      //  Use material
      else if ((str = readstr(line,"usemtl")))
         SetMaterial(str);
      //  Load materials
      else if ((str = readstr(line,"mtllib")))
         LoadMaterial(str);
      //  Skip this line
   }
   fclose(f);
   //  Pop attributes (textures)
   glPopAttrib();
   glEndList();

   //  Free materials
   for (k=0;k<Nmtl;k++)
      free(mtl[k].name);
   free(mtl);

   //  Free arrays
   free(V);
   free(T);
   free(N);

   return list;
}
