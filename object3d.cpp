#include "object3d.h"

Object3D::Object3D(){
	//Initialisation
	m_vertexArray.clear();
	m_normalArray.clear();
	m_texCoordArray.clear();
	m_triangleArray.clear();
}

Object3D::~Object3D(){
	//on clean le tout
	m_normalArray.clear();
	m_texCoordArray.clear();
	m_vertexArray.clear();
	m_triangleArray.clear();
}

Object3D::Object3D(const Object3D &o){
	//Copie simple
	m_vertexArray = std::vector<vec3>(o.m_vertexArray);
	m_normalArray = std::vector<vec3>(o.m_normalArray);
	m_texCoordArray = std::vector<vec3>(o.m_texCoordArray);
	m_triangleArray = std::vector<Triangle>(o.m_triangleArray);
}	

Object3D& Object3D::operator=(const Object3D &o){
	//Copie simple
	m_vertexArray = std::vector<vec3>(o.m_vertexArray);
	m_normalArray = std::vector<vec3>(o.m_normalArray);
	m_texCoordArray = std::vector<vec3>(o.m_texCoordArray);
	m_triangleArray = std::vector<Triangle>(o.m_triangleArray);

	return *this;
}

void Object3D::draw(){
	vec3 tmp;

	glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glBindTexture(GL_TEXTURE_2D,0);

	for(unsigned int i = 0 ; i < m_triangleArray.size() ; i++){
		glBegin(GL_TRIANGLES);
		//printf("debut gl triangles %d\n", i);
				for(int j = 0 ; j < 3 ; j++){
					tmp = m_vertexArray.at(m_triangleArray[i].Vertex[j]);
					glVertex3f(tmp.x, tmp.y, tmp.z);
					printf("glVertex3f %f %f %f\n", tmp.x, tmp.y, tmp.z);
				}
		glEnd();
	}
}


