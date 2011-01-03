#include "shaders.hpp"
#include "main.hpp"

#include <stdlib.h>
#include <string.h>

#include "tga.h"
#include "terrain.h"


static void quit(void)
{
	exit(0);
}


void drawCubeMap(float size)
{
	static GLfloat xPlane[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	static GLfloat yPlane[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	static GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	
	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
	glEnable (GL_TEXTURE_GEN_R);
	
	glEnable (GL_TEXTURE_CUBE_MAP);
	glBindTexture (GL_TEXTURE_CUBE_MAP, texid);
	
	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	
	glTexGenfv (GL_S, GL_OBJECT_PLANE, xPlane);
	glTexGenfv (GL_T, GL_OBJECT_PLANE, yPlane);
	glTexGenfv (GL_R, GL_OBJECT_PLANE, zPlane);
	
	glDisable(GL_CULL_FACE);
	glutSolidSphere (size, 30, 30);
	glEnable(GL_CULL_FACE);
	
	glDisable (GL_TEXTURE_CUBE_MAP);
	
	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);
	glDisable (GL_TEXTURE_GEN_R);
}

void changeSize(int w1, int h1)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;
	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
					x + lx,y + ly,z + lz,
				0.0f,1.0f,0.0f);
}


void initScene() 
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	
	glShadeModel(GL_SMOOTH);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glCullFace(GL_BACK);
	terrainSimulateLighting(simulateLighting);
	terrainDiffuseColor(1.0, 1.0, 1.0);
	terrainAmbientColor(0.04, 0.04, 0.04);
	terrainLightPosition(lPosition[0],lPosition[1],lPosition[2],lPosition[3]);
	terrainDL = terrainCreateDL(0,0,0,lighting);
	
	
	//Shaders
	if(!isExtensionSupported((char*)"GL_ARB_shading_language_100"))
		quit();
	
	
	GLhandleARB so[2];
	
	bzero(so,sizeof(GLhandleARB)*2);
	
	
	std::string s1 = "shaders/color.vert";
	loadShader(s1.c_str());
	so[0] = loadShader(s1.c_str());
	if (so[0]==0) {
		std::cerr << "loading shader "+s1+" failed (exiting...)" << std::endl;
		quit();
	}
	if (!compileShader(so[0])) {
		std::cerr << "compiling shader "+s1+" failed (exiting...)" << std::endl;
		quit();
	}
	
	
	std::string s2 = "shaders/color.frag";
	so[1] = loadShader(s2.c_str());
	if(so[0]==0){
		std::cerr << "loading shader "+s2+" failed (exiting...)" << std::endl;
		quit();
	}
	if(!compileShader(so[1])){
		std::cerr << "compiling shader "+s2+" failed (exiting...)" << std::endl;
		quit();
	}
		
	programobject = linkShaders(so,2);
	
	glDeleteObjectARB(so[0]);
	glDeleteObjectARB(so[1]);
	
    #ifdef CHECK_ERRORS
	err = glGetError();
	if(err!=GL_NO_ERROR){
		std::cerr << "Erreur GL :" << std::endl;
		std::cerr << gluErrorString(err) << std::endl;
	}
	#endif


	glLightfv(GL_LIGHT0,GL_AMBIENT,lAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lDiffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	
	
	//textures cubemap du ciel
	glGenTextures (1, &texid);
	if(texid==0)
	{
		printf("Erreur a l'allocation de la texture\n");
		exit(0);
	}
	
	glBindTexture (GL_TEXTURE_CUBE_MAP, texid);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	int i = 0; //pour le debug
	
	tgaInfo* image = tgaLoad((char *)"cubemap/left.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);
	
	image = tgaLoad((char *)"cubemap/down.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);
	
	image = tgaLoad((char *)"cubemap/back.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);
	
	image = tgaLoad((char *)"cubemap/right.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);
	
	image = tgaLoad((char *)"cubemap/top.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);
	
	image = tgaLoad((char *)"cubemap/front.tga"); i++;
	if(image->status != TGA_OK){
		printf("Erreur au chargement de l'image %d, erreur %d\n", i, image->status);
		exit(0);
	}
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	tgaDestroy(image);	
	
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
}

void orientMe(float ang) 
{

	lx = cos(angle2Y) * sin(ang);
	lz = - cos(angle2Y) * cos(ang);
}


void moveMeFlat(float i) 
{
	x = x + i*lx;
	z = z + i*lz;
	y = y + i*ly;
}

void setOrthographicProjection() 
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() 
{
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{	
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(font, *c);
	}
}



void renderScene(void) 
{
//	float modelview[16]; UNUSED

	//camera
	if (deltaMove)
	{
		moveMeFlat(deltaMove);
	}
	if (deltaAngle) 
	{
		angle += deltaAngle;
		orientMe(angle);
	}
	glLoadIdentity();
	gluLookAt(x, y, z, 
			  x + 10*lx,y + 10*ly,z + 10*lz,
			  0.0f,1.0f,0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//lights
	if (lighting)
	{
		glLightfv(GL_LIGHT0,GL_POSITION,lPosition);
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS,mShininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cWhite);
	if (lighting) 
	{
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	glColor3f(1,1,1);
	
	//draw sky
	glDisable(GL_LIGHTING);
	drawCubeMap(400.0);
	glEnable(GL_LIGHTING);

	//Draw ground
	glCallList(terrainDL);

	//prepare fpsq counter
	frame++;
	time_current=glutGet(GLUT_ELAPSED_TIME);
	if (time_current - timebase > 1000) 
	{
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time_current-timebase));
		timebase = time_current;		
		frame = 0;
	}
	
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glColor3f(0.0f,1.0f,1.0f); //print in teal (teal mask applied on white characters)
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30,15,(void *)font, (char*)"Terrain Tutorial @ 3D Tech"); 
	renderBitmapString(30,30,(void *)font, s); 
	renderBitmapString(30,45,(void *)font, (char*)"F1	- Game Mode	640x480 32 bits");
	renderBitmapString(30,60,(void *)font, (char*)"F2	- Game Mode	800x600 32 bits");
	renderBitmapString(30,75,(void *)font, (char*)"F3	- Game Mode 1024x768 32 bits");
	renderBitmapString(30,90,(void *)font, (char*)"F4	- Window Mode");
	renderBitmapString(30,105,(void *)font, (char*)"F10 - Simulate Lighting ON/OFF");
	renderBitmapString(30,120,(void *)font, (char*)"F11 - OpenGL Lighting ON/OFF");
	renderBitmapString(30,135,(void *)font, (char*)"F12 - Grab Screen");
	renderBitmapString(30,150,(void *)font, (char*)"Esc - Quit");
	renderBitmapString(30,165,(void *)font, currentMode);
	#ifdef CHECK_ERRORS
		err = glGetError();
		if(err!=GL_NO_ERROR){
			std::cerr << "Erreur GL :" << std::endl;
			std::cerr << gluErrorString(err) << std::endl;
		}
	#endif
	glPopMatrix();
	resetPerspectiveProjection();
	glPopAttrib();
	glColor3f(1.0f,1.0f,1.0f); //retour en couleurs normales

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	if (key == 27) 
	{
		terrainDestroy();
		exit(0);
	}
}

void pressKey(int key, int x, int y) 
{
	switch (key) 
	{
		case GLUT_KEY_LEFT : 
			deltaAngle = -0.005f;
			break;
		case GLUT_KEY_RIGHT : 
			deltaAngle = 0.005f;
			break;
		case GLUT_KEY_UP : 
			if (navigationMode == FLY)
			{
				deltaMove = 1;
			}
			else
			{
				deltaMove = 0.1;
			}
			break;
		case GLUT_KEY_DOWN : 			
			if (navigationMode == FLY)
			{
				deltaMove = -1;
			}
			else
			{
				deltaMove = -0.1;
			}
			break;
		case GLUT_KEY_F1:			
			// define resolution, color depth
			glutGameModeString("640x480:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
			{
				glutEnterGameMode();
				sprintf(gameModeString,"640x480:32");
				w = 640;
				h = 480;
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F2:		 
			// define resolution, color depth
			glutGameModeString("800x600:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
			{
				glutEnterGameMode();
				sprintf(gameModeString,"800x600:32");
				w = 800;
				h = 600;
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F3:	
			// define resolution, color depth
			glutGameModeString("1024x768:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
			{
				glutEnterGameMode();
				w = 1024;
				h = 768;
				sprintf(gameModeString,"1024x768:32");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F4:	
			// return to default window
			w = 640;h = 360;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			break;
		case GLUT_KEY_F5:
			bla = !bla;
			break;
		case GLUT_KEY_F10:
			simulateLighting = !simulateLighting;
			terrainSimulateLighting(simulateLighting);
			glDeleteLists(terrainDL,1);
			if (simulateLighting)
				lighting = 0;
			if (lighting) 
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			else 
			{
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHTING);
			}
			terrainDL = terrainCreateDL(0,0,0,0);
			break;
		case GLUT_KEY_F11:
			lighting = !lighting;
			if (lighting) 
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			else 
			{
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHTING);
			}
			glDeleteLists(terrainDL,1);
			if (lighting)
				simulateLighting = 0;
			terrainDL = terrainCreateDL(0,0,0,lighting);
			break;
		case GLUT_KEY_F12:
			tgaGrabScreenSeries((char*)"3dtechscreen",0,0,w,h);
			break;
		
		default :
			printf("useless key pressed");
			break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(currentMode,"Current Mode: Window - lighting : %d simulate lighting %d",lighting,simulateLighting);
	else
	{
		sprintf(currentMode,
			"Current Mode: Game Mode %dx%d at %d hertz, %d bpp - lighting : %d simulate lighting %d",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH),
			lighting,simulateLighting);
	}
}

void releaseKey(int key, int x, int y) 
{
	switch (key) 
	{
		case GLUT_KEY_LEFT : 
			if (deltaAngle < 0.0f) 
				deltaAngle = 0.0f;
			break;
		case GLUT_KEY_RIGHT : 
			if (deltaAngle > 0.0f) 
				deltaAngle = 0.0f;
			break;
		case GLUT_KEY_UP :	 
			if (deltaMove > 0) 
				deltaMove = 0;
			break;
		case GLUT_KEY_DOWN : 
			if (deltaMove < 0) 
				deltaMove = 0;
			break;
	}
}


void activeMouseMotion(int x, int y) 
{
	angle2 = angle + (x-deltaX)*0.001;
	angle2Y = angleY + (y-deltaY) * 0.001;
	if (angle2Y > 1.57)
		angle2Y = 1.57;
	else if (angle2Y < -1.57)
		angle2Y = -1.57;
	lx = cos(angle2Y)*sin(angle2);
	lz = -cos(angle2Y)*cos(angle2);
	ly = -sin(angle2Y);
}


void mousePress(int button, int state, int x, int y) 
{
	if (state == GLUT_DOWN) 
	{
//		angle2 = 0;
		deltaX = x;
		deltaY = y;
//		angle2Y = 0;
		navigationMode = FLY;
	} 
	else if (state == GLUT_UP) 
	{
		angleY = angle2Y;
		angle = angle2;
		navigationMode = WALK;
	}
}

void init() 
{
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMotionFunc(activeMouseMotion);
	glutMouseFunc(mousePress);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	initScene();

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("SnowMen from 3D-Tech");

	// init terrain structures
	if (terrainLoadFromImage((char*)"3dtech.tga",1) != TERRAIN_OK)
		return(-1);
	terrainScale(0,20);
	// register all callbacks and
	// create display lists
	init();

	printf("load d'un fichier obj\n");
	ObjLoader obj = ObjLoader("tmb_1.obj");
	printf("affichage d'un fichier ob\n");
	obj.returnObj().draw();
	printf("import fini");

	glutMainLoop();

	return(0);
}

