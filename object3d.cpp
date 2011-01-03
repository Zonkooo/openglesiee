#include "object3d.h"

Object3D::Object3D(){
	//Initialisation
	m_numVertex = 0;
	m_numNormal = 0;
	m_numTexCoord = 0;
	m_numTriangle = 0;
	
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
	//reinit
	m_vertexArray.resize(o.m_numVertex);
	m_normalArray.resize(o.m_numNormal);
	m_texCoordArray.resize(o.m_numTexCoord);
	m_triangleArray.resize(o.m_numTriangle);
	
	m_numVertex = o.m_numVertex;
	m_numNormal = o.m_numNormal;
	m_numTexCoord = o.m_numTexCoord;
	m_numTriangle = o.m_numTriangle;

	//Copie simple
	for(int i = 0 ; i < m_numNormal ; i++)
		m_normalArray[i] = o.m_normalArray[i];

	for(int i = 0 ; i < m_numTexCoord ; i++)
		m_texCoordArray[i] = o.m_texCoordArray[i];

	for(int i = 0 ; i < m_numTriangle ; i++)
		m_triangleArray[i] = o.m_triangleArray[i];

	for(int i = 0 ; i < m_numVertex ; i++)
		m_vertexArray[i] = o.m_vertexArray[i];

}	

Object3D& Object3D::operator=(const Object3D &o){
	//on clean le tout au cas ou
	m_normalArray.clear();
	m_texCoordArray.clear();
	m_vertexArray.clear();
	m_triangleArray.clear();

	//reinit
	m_vertexArray.resize(o.m_numVertex);
	m_normalArray.resize(o.m_numNormal);
	m_texCoordArray.resize(o.m_numTexCoord);
	m_triangleArray.resize(o.m_numTriangle);
	
	m_numVertex = o.m_numVertex;
	m_numNormal = o.m_numNormal;
	m_numTexCoord = o.m_numTexCoord;
	m_numTriangle = o.m_numTriangle;

	//Copie simple
	for(int i = 0 ; i < m_numNormal ; i++)
		m_normalArray[i] = o.m_normalArray[i];

	for(int i = 0 ; i < m_numTexCoord ; i++)
		m_texCoordArray[i] = o.m_texCoordArray[i];

	for(int i = 0 ; i < m_numTriangle ; i++)
		m_triangleArray[i] = o.m_triangleArray[i];

	for(int i = 0 ; i < m_numVertex ; i++)
		m_vertexArray[i] = o.m_vertexArray[i];

	return *this;
}

void Object3D::draw(){
	vec3 tmp;
	for(int i = 0 ; i < m_numTriangle ; i++){
		printf("debut draw\n");
		glBegin(GL_TRIANGLES);
				for(int j = 0 ; i < 3 ; i++){
					tmp = m_vertexArray.at(m_triangleArray[i].Vertex[j]);
					glVertex3f(tmp.x, tmp.y, tmp.z);
					printf("glVertex3f %f %f %f\n", tmp.x, tmp.y, tmp.z);
				}
		glEnd();
	}
}


