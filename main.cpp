#include "shaders.h"
#include "main.h"

#include <stdlib.h>
#include <string.h>

#include "tga.h"
#include "terrain.h"

// template ////
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

////

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

void mat_inverse (float *in, float *out)
{
		float det, oneOverDet;

	det = in[0] * ((in[5] * in[10]) - (in[6] * in[9]))
		+ in[1] * ((in[6] * in[8]) - (in[4] * in[10]))
		+ in[2] * ((in[4] * in[9]) - (in[5] * in[8]));

	oneOverDet = 1.0f / det;

	out[0] = ((in[5] * in[10])- (in[6] * in[9])) * oneOverDet;
	out[1] = ((in[2] * in[9]) - (in[1] * in[10]))* oneOverDet;
	out[2] = ((in[1] * in[6]) - (in[2] * in[5])) * oneOverDet;
	out[3] = 0.0f;

	out[4] = ((in[6] * in[8]) - (in[4] * in[10]))* oneOverDet;
	out[5] = ((in[0] * in[10])- (in[2] * in[8])) * oneOverDet;
	out[6] = ((in[2] * in[4]) - (in[0] * in[6])) * oneOverDet;
	out[7] = 0.0f;

	out[8] = ((in[4] * in[9]) - (in[5] * in[8])) * oneOverDet;
	out[9] = ((in[1] * in[8]) - (in[0] * in[9])) * oneOverDet;
	out[10]= ((in[0] * in[5]) - (in[1] * in[4])) * oneOverDet;
	out[11]= 0.0f;

	out[12] = 0.0f;
	out[13] = 0.0f;
	out[14] = 0.0f;
	out[15] = 1.0f;
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
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,lAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lDiffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	
	//chargement des obj
	printf("load d'un fichier obj\n");
	ol.loadObj("obj/ele1.obj");
	printf("fin load d'un fichier obj\n");

	obj = ol.returnObj();
	//vec3 *c = new vec3(0.8078, 0.2745, 0.4627);
	//obj.setColor(c);
	vec3 *p = new vec3(200.0,0.0,0.0);
	obj.setPos(p);
//	obj.loadTexture((char*)"elephant_tex.tga");
	printf("triangles : %d, vertex : %d\n", (int)obj.m_triangleArray.size(), (int)obj.m_vertexArray.size());
	
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
	
	glBindTexture (GL_TEXTURE_CUBE_MAP, texid);
	
	//----------------------------------------
	//             Shaders
	//----------------------------------------
	printf("debut chargmenent des shaders\n");
	if(!isExtensionSupported((char*)"GL_ARB_shading_language_100"))
		exit(0);
	
	GLhandleARB so[2];	
	bzero(so,sizeof(GLhandleARB)*2);	
	
	const char* s1 = "shaders/water.vert";
	so[0] = loadShader(s1);
	if (so[0]==0) {
		std::cerr << "loading shader " << s1 << " failed (exiting...)" << std::endl;
		exit(0);
	}
	if (!compileShader(so[0])) {
		std::cerr << "compiling shader " << s1 << " failed (exiting...)" << std::endl;
		exit(0);
	}
	
	std::string s2 = "shaders/water.frag";
	so[1] = loadShader(s2.c_str());
	if(so[0]==0){
		std::cerr << "loading shader "+s2+" failed (exiting...)" << std::endl;
		exit(0);
	}
	if(!compileShader(so[1])){
		std::cerr << "compiling shader "+s2+" failed (exiting...)" << std::endl;
		exit(0);
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
}

//camera thib

void loadCamMvt(char* fileName)
{
	std::string tempString;
	
	
	int strPtr, strPtr2;
	int tabPtr = 0;
	
	camFile.open(fileName);
	camFile.is_open();
	
	if(camFile.fail())
		return;
	
	while (1) 
	{
		getline(camFile, camFileLine);
		if(camFile.eof())
			break;
		
		if(camFileLine[0]=='#') //comments
			continue;
			
		std::stringstream str;
		strPtr=0;
		strPtr2=0;
		
		
		while (camFileLine[strPtr]!='\t') {
			strPtr++;
		}
		from_string<int>(mvtTab[tabPtr].time, camFileLine.substr(0,strPtr), std::dec);
		mvtTab[tabPtr].time = 1000 * mvtTab[tabPtr].time;
		
		strPtr2 = ++ strPtr;
		
		while (camFileLine[strPtr]!='\t') {
			strPtr++;
		}
		from_string<float>(mvtTab[tabPtr].moveFrontBack, camFileLine.substr(strPtr2,strPtr-strPtr2), std::dec);
		
		strPtr2 = ++ strPtr;
		
		while (camFileLine[strPtr]!='\t') {
			strPtr++;
		}
		from_string<float>(mvtTab[tabPtr].rotUpDown, camFileLine.substr(strPtr2,strPtr-strPtr2), std::dec);
		
		strPtr2 = ++ strPtr;
		
		while (camFileLine[strPtr]!='\t' && camFileLine[strPtr]!='\0') { 
			strPtr++;
		}
		from_string<float>(mvtTab[tabPtr].rotLeftRight, camFileLine.substr(strPtr2,strPtr-strPtr2), std::dec);
		
		//tempString = camFileLine.substr(strPtr2,strPtr-strPtr2);
		//std::cout << tempString << std::endl;
	
		//from_string<int>(mvtTab[tabPtr].time, camFileLine.substr(0,strPtr), std::dec);
		//from_string<float>(mvtTab[tabPtr].moveFrontBack, camFileLine.substr(strPtr2,strPtr-strPtr2), std::dec);
		
		//tempString = camFileLine.substr(strPtr2,strPtr-strPtr2);
		//str << tempString;
		//str >> mvtTab[tabPtr].time;
		
		//std::cout << mvtTab[tabPtr].time << std::endl;
		
		tabPtr++;
	}
	
	printf("next\n");
	for (int i = 0; i<10; i++) {
		std::cout << mvtTab[i].time << '\t' << mvtTab[i].moveFrontBack << '\t' << mvtTab[i].rotUpDown << '\t' << mvtTab[i].rotLeftRight << std::endl;
	}
	
	
}

void camera (void) {
	
    glRotatef(xrot,1.0,0.0,0.0);  //rotate cam left and right
    glRotatef(yrot,0.0,1.0,0.0);  //rotate cam up and down
    glTranslated(-xpos,-ypos,-zpos); //translate cam
	
}

void moveCamera(float moveFrontBack, float rotUpDown, float rotLeftRight)
{
	float xrotrad, yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f); 
	xpos += moveFrontBack*float(sin(yrotrad)) ;
	zpos -= moveFrontBack*float(cos(yrotrad)) ;
	ypos -= moveFrontBack*float(sin(xrotrad)) ;
	
	
	xrot -= rotUpDown;
	if (xrot >360) xrot -= 360;
	else if (xrot < -360) xrot += 360;
	
	yrot -= rotLeftRight;
	if (yrot >360) yrot -= 360;
	else if (yrot < -360)yrot += 360;
}

void cameraManager(int simTime)
{
	if(simTime >= mvtTab[mvtPtr].time)
	{
		mvtPtr++;
		//std::cout << "changement de mouvement : " << simTime << std::endl;
	}
	int timeDelta = simTime - camTimeVar;
	//printf("%d\n",timeDelta);
	float flatmvt = (timeDelta * mvtTab[mvtPtr].moveFrontBack)/(mvtTab[mvtPtr].time - simTime);
	float vertrot = (timeDelta * mvtTab[mvtPtr].rotUpDown)/(mvtTab[mvtPtr].time - simTime);
	float flatrot = (timeDelta * mvtTab[mvtPtr].rotLeftRight)/(mvtTab[mvtPtr].time - simTime);
	moveCamera(flatmvt,vertrot,flatrot);
	
	mvtTab[mvtPtr].moveFrontBack -= flatmvt;
	mvtTab[mvtPtr].rotUpDown	-= vertrot;
	mvtTab[mvtPtr].rotLeftRight -= flatrot;
	
		
	camTimeVar = simTime;
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
	//camera
	cameraManager(time_current);
	camera();
	glLoadIdentity();
	camera();
	
//	if (deltaMove)
//	{
//		moveMeFlat(deltaMove);
//	}
//	if (deltaAngle) 
//	{
//		angle += deltaAngle;
//		orientMe(angle);
//	}
//	glLoadIdentity();
//	gluLookAt(x, y, z, 
//			  x + 10*lx,y + 10*ly,z + 10*lz,
//			  0.0f,1.0f,0.0f);

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
	glDisable(GL_DEPTH_TEST);
	drawCubeMap(400.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//import de porc
	//printf("affichage d'un fichier obj\n");
    glMatrixMode(GL_MODELVIEW);
    glScalef(0.1f, 0.1f, 0.1f);
		glTranslatef(0.0f, 36.f, -1000.f);
			glDisable(GL_CULL_FACE);
				glDisable(GL_LIGHTING);
					obj.draw();
				glEnable(GL_LIGHTING);
			glEnable(GL_CULL_FACE);
		glTranslatef(0.0f, -36.f, 1000.f);
    glScalef(10.f, 10.f, 10.f);
    glFlush();
	//printf("draw obj fini\n");

	//Draw ground
	glCallList(terrainDL);
	
	//build de la matrice modelview
	float mat[16];
	float modelview[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, mat);
	mat_inverse (mat, modelview); //matrice de passage de camera à scene
	
	//draw water
//	glColor3f(1.0f, 0.0f, 1.0f);
	glUseProgramObjectARB(programobject); //shader mode
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, texid);
	glUniform1i(glGetUniformLocation(programobject, "sky"), 0); 
	
	glUniformMatrix4fv(glGetUniformLocation(programobject, "cam_to_scene"), 1, false, modelview);
		
	glBegin(GL_POLYGON);
		glVertex3f(-150.0f, 10.0f, -250.0f);
		glVertex3f(-150.0f, 10.0f,  -50.0f);
		glVertex3f( 150.0f, 10.0f,  -50.0f);
		glVertex3f( 150.0f, 10.0f, -250.0f);
	glEnd();
	
	glUseProgramObjectARB(0); //fin des shaders

	//prepare fps counter
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
	int hauteur = 15;
	renderBitmapString(30,hauteur,(void *)font, s); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, (char*)"F1  - 640x480"); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, (char*)"F2  - 800x600"); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, (char*)"F3  - 1024x768"); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, (char*)"F10 - Simulate Lighting ON/OFF"); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, (char*)"F11 - OpenGL Lighting ON/OFF"); hauteur += 15;
	renderBitmapString(30,hauteur,(void *)font, currentMode); hauteur += 15;
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

#include <unistd.h>
static void idleGL(void)
{
	//on attend un peu pour pas bouriner en fps
	usleep(10000); //en microsecondes
	
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	// camera //
	
	
	if (key == 'f') //down
	{	

		moveCamera(0,-1,0);
	
	}
	
	if (key == 'r') //up
	{

			moveCamera(0,1,0);
	
	}
	
	if (key == 'z') //front
	{
		moveCamera(1,0,0);

	}
	
	if (key == 's') //back
	{
		moveCamera(-1,0,0);
		
	}
	
	if (key == 'd') //right
	{
			moveCamera(0,0,-1);
			
	}
	
	if (key == 'q') //left
	{
			moveCamera(0,0,1);
	}
	
	if (key == 27) 
	{
		terrainDestroy();
		exit(0);
	}
	else if(key =='c')
	{ //prints the current camera position
		printf("camera :\neye : (%f, %f, %f)\ncenter : (%f, %f, %f)\n\n", (float)x, (float)y, (float)z, x + 10*lx, y + 10*ly, z + 10*lz);
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
	glewInit();
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMotionFunc(activeMouseMotion);
	glutMouseFunc(mousePress);
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idleGL);
	initScene();

}

int main(int argc, char **argv)
{
	loadCamMvt((char*)"config.cam");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("Projet OpenGL - team ESIEE");

	// init terrain structures
	if (terrainLoadFromImage((char*)"3dtech.tga",1) != TERRAIN_OK)
		return(-1);
	terrainScale(0,20);
	// register all callbacks and
	// create display lists
	init();

	glutMainLoop();

	return(0);
}

