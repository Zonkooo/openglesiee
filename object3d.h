#pragma once
/*
Classe Objet3D
Author: Clement Robein

Classe de stockage des modèles 3D au format OBJ

*/

#include <stdio.h>
#include <vector>
#include "vec3.h"

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

struct Triangle{
	int Vertex[3];
	int Normal[3];
	int TexCoord[3];
};

class Object3D {
	public:
		Object3D();
		~Object3D();
		
		Object3D(const Object3D& o);
		Object3D& operator=(const Object3D& o);
		void draw();
		void setColor(vec3 *c);

		std::vector<vec3> m_vertexArray;
		std::vector<vec3> m_normalArray;
		std::vector<vec3> m_texCoordArray;
		std::vector<Triangle> m_triangleArray;

		vec3 m_c;
};

