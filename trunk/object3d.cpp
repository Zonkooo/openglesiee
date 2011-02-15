#include "object3d.h"

Object3D::Object3D(){
	//Initialisation
	m_vertexArray.clear();
	m_normalArray.clear();
	m_texCoordArray.clear();
	m_triangleArray.clear();
	m_materialArray.clear();
}

Object3D::~Object3D(){
	//on clean le tout
	m_normalArray.clear();
	m_texCoordArray.clear();
	m_vertexArray.clear();
	m_triangleArray.clear();
	m_materialArray.clear();
}

Object3D::Object3D(const Object3D &o){
	//Copie simple
	m_vertexArray = std::vector<vec3>(o.m_vertexArray);
	m_normalArray = std::vector<vec3>(o.m_normalArray);
	m_texCoordArray = std::vector<vec3>(o.m_texCoordArray);
	m_triangleArray = std::vector<Triangle>(o.m_triangleArray);
	m_materialArray = std::vector<Material>(o.m_materialArray);
}	

Object3D& Object3D::operator=(const Object3D &o){
	//Copie simple
	m_vertexArray = std::vector<vec3>(o.m_vertexArray);
	m_normalArray = std::vector<vec3>(o.m_normalArray);
	m_texCoordArray = std::vector<vec3>(o.m_texCoordArray);
	m_triangleArray = std::vector<Triangle>(o.m_triangleArray);
	m_materialArray = std::vector<Material>(o.m_materialArray);

	return *this;
}

void Object3D::draw(){
	vec3 tmp;
	Material *curMat;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glBindTexture(GL_TEXTURE_2D,0);

	for(unsigned int i = 0 ; i < m_triangleArray.size() ; i++){

		for(unsigned int k = 0; k < m_materialArray.size() ; k++)
        {
       	    if((unsigned int)m_materialArray[k].firstVertice == i)
       	    {
				/*glColor3f(m_materialArray[k].diffu.x, m_materialArray[k].diffu.y, m_materialArray[k].diffu.z);
				glDisable(GL_COLOR_MATERIAL);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &m_materialArray[k].amb.x);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &m_materialArray[k].diffu.x);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &m_materialArray[k].spec.x);
				glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, m_materialArray[k].Ns);*/

        	    if(m_materialArray[k].tex)
        	    	glBindTexture(GL_TEXTURE_2D, m_materialArray[k].m_texture);
        	    curMat = &m_materialArray[k];
        	    break;
            }
        }

		glBegin(GL_TRIANGLES);
		for(int j = 0 ; j < 3 ; j++){
			/*if(curMat->tex)
			{
	printf("truc0\n");
				tmp = m_vertexArray.at(m_triangleArray[i].TexCoord[j]);
				glTexCoord2f(tmp.x, tmp.y);
			}*/

			tmp = m_vertexArray.at(m_triangleArray[i].Vertex[j]);
			glVertex3f(tmp.x+m_pos.x, tmp.y+m_pos.y, tmp.z+m_pos.z);
		}
		glEnd();
	}

	glPopMatrix();
}

void Object3D::setColor(vec3 *c)
{
	m_c = *c;
}

void Object3D::setPos(vec3 *p)
{
	m_pos = *p;
}

/*void Object3D::loadTexture(char* fileName)
{
	// open file
	tgaInfo* tex = tgaLoad(fileName);
	if(tex->status != TGA_OK){
		printf("Erreur au chargement de la texture : erreur %d\n", tex->status);
		exit(0);
	}
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->imageData);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// memory cleaning
	tgaDestroy(tex);
}*/

