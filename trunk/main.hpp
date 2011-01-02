#ifdef LINUX
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#ifdef MAC
#include <glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#endif

#define CHECK_ERRORS

#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <string>

#include "tga.h"
#include "terrain.h"



int bla = 0;

GLuint texid = 0; //textures du ciel

// stuff for lighting
GLfloat lAmbient[] = {0.2,0.2,0.2,1.0};
GLfloat lDiffuse[] = {1.0,1.0,1.0,1.0};
GLfloat lSpecular[] = {1.0,1.0,1.0,1.0};

GLfloat lPosition[] = {0.0, 100.0, 0.0, 0.0};

// materials
GLfloat mSpecular[] = {0.0,0.0,0.0,0.0};
// the smaller the larger the specular area is
GLfloat mShininess[] = {128.0};

//colors
GLfloat cBlack[] = {0.0,0.0,0.0,1.0};
GLfloat cOrange[] = {1.0,0.5,0.5,1.0}; 
GLfloat cWhite[] = {1.0,1.0,1.0,1.0}; 
GLfloat cGrey[] = {0.1,0.1,0.1,1.0};
GLfloat cLightGrey[] = {0.9,0.9,0.9,1.0};

int lighting=1,simulateLighting = 0;

/* this is not doing anything at the moment */
#define FLY		1
#define WALK	2
int navigationMode = WALK;


float angle=0.0,deltaAngle = 0.0,ratio;
float x=0.0f,y=1.75f,z=5.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f,deltaMove=0.0;
int h,w;
void* font = GLUT_BITMAP_8_BY_13;	
int bitmapHeight=13;
int mode;
float angle2,angle2Y,angleY;
static int deltaX=-1000,deltaY;

int terrainDL,iterations = 0,totalIterations = 0;
char s[100];

int frame,time_current,timebase=0;
char currentMode[100];

// this string keeps the last good setting 
// for the game mode
char gameModeString[40] = "640x480";

// Origine de la fenetre
static const unsigned int windowx = 0;
static const unsigned int windowy = 0;

// Dimensions de la fenetre
static unsigned int windowwidth  = 800;
static unsigned int windowheight = 600;

GLenum err;

// Modele de la camera
static float position[] = {0.0f,0.0f,3.0f};
static float aim[] = {0.0f,0.0f,-1.0f};
static float direction[] = {0.0f,0.0f,-1.0f};

// Rotation de la camera
static float yrotation = 0.0f;
static float xrotation = 0.0f;

// Données sur le curseur et la souris
static int xmouseposition = 0;
static int ymouseposition = 0;
static bool buttonpressed = false;

GLhandleARB programobject = 0;
