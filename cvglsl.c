/*
 *
 *  Peter R. Elespuru
 *
 *  Derived from examples, other attributions in line as necessary
 *  primarily started with this: http://www.prinmath.com/csci5229/Sp12/programs/ex13.zip
 *
 *  Real Time Image Processing
 *
 *  'm' to switch filters
 *  's' saves a screenshot of current view
 *  arrows pan
 *  PgUp/PgDn zooms in/out
 * 
 */
	 
#include "cvglsl.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string.h>
#include <stdio.h>
	 
int mode=0;         //  Filter to use
int axes=0;         //  Draw crosshairs
double asp=1;       //  Aspect ratio
double zoom=1;      //  Zoom factor
int   N=1;          //  Passes
int   W0,H0;        //  Capture dimensions
int   W1,H1;        //  Window dimensions
float X=0,Y=0;      //  Initial position
CvCapture*   cv;    //  OpenCV context
unsigned int cvtex; //  OpenCV texture
unsigned int imtex; //  Image texture
#define MODE 28
int shader[MODE] = {0}; //  Shader programs
char* text[] = {"No Shader","Copy","Sharpen","Blur","Erosion","Dilation","Laplacian Edge Detection","Prewitt Edge Detection","Sobel Edge Detection","Vignette", "Cross-Hatch","Colored-Cross-Hatch", "NightVision", "ThermalVision", "Posterized", "DreamVision","Pixelation","Toon","Billboard","Grayscale","Sepia","Negative","Halftone","HexPixelation","LineDrawing","LineDrawing2","Scanlines","Worhol"};

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int k;
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT);

   //  Set projection
   glLoadIdentity();
   Project(0,asp,1.0);

   //  Set up for drawing
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Set shader
   if (mode>0)
   {
      glUseProgram(shader[mode]);
      int id = glGetUniformLocation(shader[mode],"img");
      if (id>=0) glUniform1i(id,0);
   }

   //  Draw shader passes to a quad
   for (k=0;k<N;k++)
   {
      //  Quad width
      float w = (k==0) ? 1 : asp;
      glPushMatrix();
      //  Initial pass - camera image
      if (k==0)
      {
         glBindTexture(GL_TEXTURE_2D,cvtex);
         glScaled(zoom,zoom,1);
         glTranslated(X,Y,0);
         glRotatef(180,0,0,1);
      }
      //  Repeat pass - screen mage
      else
      {
         glBindTexture(GL_TEXTURE_2D,imtex);
         glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,0,0,W1,H1,0); 
      }

      //  Set offsets
      if (mode>0)
      {
         float dX = (k==0) ? 1.0/W0 : zoom/W1;
         float dY = (k==0) ? 1.0/H0 : zoom/H1;
         int id = glGetUniformLocation(shader[mode],"dX");
         if (id>=0) glUniform1f(id,dX);
         id = glGetUniformLocation(shader[mode],"dY");
         if (id>=0) glUniform1f(id,dY);
      }

      //  Redraw the texture
      glClear(GL_COLOR_BUFFER_BIT);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(-w,-1);
      glTexCoord2f(0,1); glVertex2f(-w,+1);
      glTexCoord2f(1,1); glVertex2f(+w,+1);
      glTexCoord2f(1,0); glVertex2f(+w,-1);
      glEnd();
      glPopMatrix();
   }
   glDisable(GL_TEXTURE_2D);

   //  Shader off
   glUseProgram(0);

   //  Draw crosshairs
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex2f(-0.1,0);
      glVertex2f(+0.1,0);
      glVertex2f(0,-0.1);
      glVertex2f(0,+0.1);
      glEnd();
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Zoom=%.1f Offset=%f,%f Mode=%s Passes=%d",
     zoom,X,Y,text[mode],N);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      X -= 0.03/zoom;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      X += 0.03/zoom;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      Y -= 0.03/zoom;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      Y += 0.03/zoom;
   //  PageUp key - increase zoom
   else if (key == GLUT_KEY_PAGE_DOWN)
      zoom /= 1.1;
   //  PageDown key - decrease zoom
   else if (key == GLUT_KEY_PAGE_UP)
      zoom *= 1.1;
   //  Limit zoom
   if (zoom<1)
   {
      zoom = 1;
      X = Y = 0;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view
   else if (ch == '0')
      X = Y = 0;
   //  Cycle modes
   else if (ch == 'm')
      mode = (mode+1)%MODE;
   else if (ch == 'M')
      mode = (mode+MODE-1)%MODE;
   //  Passes
   else if (ch == 'N' && N>1)
      N--;
   else if (ch == 'n')
      N++;
   //  Toggle axes
   else if (ch == 'a')
      axes = !axes;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set image dimensions
   W1 = width;
   H1 = height;
}

/*
 *  Function called to capture images at 20ms intervals
 */
void capture(int k)
{
   //  Capture image
   IplImage* img = cvQueryFrame(cv); 
   if (!img) Fatal("Image capture failed\n");

   /* tests
//   printf(":%d\n",img->);
   printf("nSize:%d\n",img->nSize);
   printf("nChannels:%d\n",img->nChannels);
   printf("alphaChannel:%d\n",img->alphaChannel);
   printf("depth:%d\n",img->depth);
   printf("colorModel:%s\n",img->colorModel);
   printf("channelSeq:%s\n",img->channelSeq);
   printf("dataOrder:%d\n",img->dataOrder);
   printf("origin:%d\n",img->origin);
   printf("align:%d\n",img->align);
   printf("step:%d\n",img->widthStep);
   printf("width:%d\n",img->width);
   printf("channels:%d\n",img->nChannels);
   printf("---\n\n");
   //cvShowImage("Live Cam", img);
   */
   
   //  Copy image to texture
   glBindTexture(GL_TEXTURE_2D,cvtex);
   
   int mode = GL_RGB;
   int type = GL_UNSIGNED_BYTE;
   
   if (strcmp(img->channelSeq,"BGR") == 0) {
	   mode = GL_BGR;
   }
   
   // functional mac approach is from here:
   // http://www.alecjacobson.com/weblog/?p=1875
   // Image is memory aligned which means we there may be extra space at the end
   // of each row. gluBuild2DMipmaps needs contiguous data, so we buffer it here
   char * buffer = malloc (sizeof(char) * img->width * img->height * img->nChannels);
   int step     = img->widthStep;
   int height   = img->height;
   int width    = img->width;
   int channels = img->nChannels;
   char * data  = (char *)img->imageData;
   int i = 0;
   for(;i<height;i++) {
  	 memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
   }
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->width,img->height,0,mode,type,buffer);
   
   ErrCheck("Capture");
   W0 = img->width;
   H0 = img->height;

   //  Set timer
   glutTimerFunc(20,capture,0);

   //  Display
   glutPostRedisplay();
   
   free(buffer);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600, 600);
   glutCreateWindow("CVGLSL");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Initialize OpenCV
   cv = cvCreateCameraCapture(0);
   if (!cv) Fatal("Could not initialize OpenCV\n");
   //  Texture to store image
     
   glGenTextures(1,&cvtex);
   glBindTexture(GL_TEXTURE_2D,cvtex);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   
   //  Texture to post-process image
   glGenTextures(1,&imtex);
   glBindTexture(GL_TEXTURE_2D,imtex);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   //  Start image capture
   capture(0);
   
   //  Create Shader Programs
   shader[1] = CreateShaderProg(NULL,"copy.frag");
   shader[2] = CreateShaderProg(NULL,"sharpen.frag");
   shader[3] = CreateShaderProg(NULL,"blur.frag");
   shader[4] = CreateShaderProg(NULL,"erosion.frag");
   shader[5] = CreateShaderProg(NULL,"dilation.frag");
   shader[6] = CreateShaderProg(NULL,"laplacian.frag");
   shader[7] = CreateShaderProg(NULL,"prewitt.frag");
   shader[8] = CreateShaderProg(NULL,"sobel.frag");
   
   shader[9] = CreateShaderProg(NULL,"vignette.frag");
   shader[10] = CreateShaderProg(NULL,"crosshatch.frag");
   shader[11] = CreateShaderProg(NULL,"crosshatch2.frag");
   shader[12] = CreateShaderProg(NULL,"nvscope.frag");
   shader[13] = CreateShaderProg(NULL,"thermal.frag");
   shader[14] = CreateShaderProg(NULL,"posterize.frag");
   shader[15] = CreateShaderProg(NULL,"dream.frag");
   shader[16] = CreateShaderProg(NULL,"pixelation.frag");
   shader[17] = CreateShaderProg("toon.vert","toon.frag");
   shader[18] = CreateShaderProg("billboard.vert","billboard.frag");
   shader[19] = CreateShaderProg("grayscale.vert","grayscale.frag");
   shader[20] = CreateShaderProg("sepia.vert","sepia.frag");
   shader[21] = CreateShaderProg("invert.vert","invert.frag");
   shader[22] = CreateShaderProg(NULL,"halftone.frag");
   shader[23] = CreateShaderProg(NULL,"hexpix.frag");
   shader[24] = CreateShaderProg("line.vert","line.frag");
   shader[25] = CreateShaderProg("line2.vert","line2.frag");
//   shader[26] = CreateShaderProg("roto.vert","roto.frag"); // CANT GET THIS ONE WORKING, PUNTING FOR NOW
   shader[26] = CreateShaderProg("scanlines.vert","scanlines.frag");
   shader[27] = CreateShaderProg("worhol.vert","worhol.frag");
   
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
