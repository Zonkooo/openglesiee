#pragma once

#ifdef LINUX
#include <GL/glew.h>
#include <GL/gl.h>
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
#include "objloader.h"


int bla = 0;

GLuint texid = 0; //textures du ciel

// camera 
//typedef struct cameraMvt;

struct cameraMvt {
	int time;
	float moveFrontBack;
	float rotUpDown;
	float rotLeftRight;
};

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0;
int camTimeVar=0;
int mvtStartTime = 0;
int mvtPtr = 0;

cameraMvt mvtTab[1024];

std::ifstream camFile;
std::string camFileLine;


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
float x = 0.0f, y = 50.f, z = 0.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f,deltaMove=0.0;
int h,w;
void* font = GLUT_BITMAP_8_BY_13;	
int bitmapHeight=13;
int mode;
float angle2,angle2Y,angleY;
static int deltaX=-1000,deltaY;

int terrainDL,iterations = 0,totalIterations = 0;
char s[100];

//import obj
ObjLoader ol;
Object3D obj;

int frame,time_current,timebase=0;
char currentMode[100];

// this string keeps the last good setting 
// for the game mode
char gameModeString[40] = "640x480";

GLenum err;
GLhandleARB programobject = 0;

void init();


