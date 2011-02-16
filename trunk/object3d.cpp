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
	Material *curMat = NULL;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, 0);

	for(unsigned int i = 0 ; i < m_triangleArray.size() ; i++)
	{
		for(unsigned int k = 0; k < m_materialArray.size() ; k++)
        {
       	    if((unsigned int)m_materialArray[k].firstFace == i)
       	    {
				glColor3f(m_materialArray[k].diffu.x, m_materialArray[k].diffu.y, m_materialArray[k].diffu.z);
				glDisable(GL_COLOR_MATERIAL);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &m_materialArray[k].amb.x);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &m_materialArray[k].diffu.x);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &m_materialArray[k].spec.x);
				glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, m_materialArray[k].Ns);

        	    if(m_materialArray[k].tex){
        	    	glBindTexture(GL_TEXTURE_2D, m_materialArray[k].m_texture);
	//				printf("%d\n", m_materialArray[k].m_texture);
				}
				else{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
        	    curMat = &m_materialArray[k];
        	    break;
            }
        }

		glBegin(GL_TRIANGLES);
		for(int j = 0 ; j < 3 ; j++){
			if(curMat != NULL && curMat->tex)
			{
				tmp = m_texCoordArray.at(m_triangleArray[i].TexCoord[j]);
				glTexCoord2f(tmp.x, tmp.y);
	//			printf("%f %f\n", tmp.x, tmp.y);
			}

			tmp = m_vertexArray.at(m_triangleArray[i].Vertex[j]);
			glVertex3f(tmp.x+m_pos.x, tmp.y+m_pos.y, tmp.z+m_pos.z);
		}
		glEnd();
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Object3D::setColor(vec3 *c)
{
	m_c = *c;
}

void Object3D::setPos(vec3 *p)
{
	m_pos = *p;
}

void Object3D::loadTextures()
{
	for(unsigned int i = 0 ; i < m_materialArray.size() ; i++)
	{
		//printf("%d\n%d\n%s\n\n", m_materialArray.size(), m_materialArray[i].firstFace, m_materialArray[i].texName.c_str());
		
		if(m_materialArray[i].texName == "")
			continue;
		// open file
		char s[256];
		strcpy(s, m_materialArray[i].texName.c_str());
		tgaInfo* tex = tgaLoad(s);
		if(tex->status != TGA_OK){
			printf("Erreur au chargement de la texture : erreur %d\n", tex->status);
			exit(0);
		}
		glGenTextures(1, &m_materialArray[i].m_texture);
		glBindTexture(GL_TEXTURE_2D, m_materialArray[i].m_texture);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->imageData);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// memory cleaning
		tgaDestroy(tex);
	}
}

