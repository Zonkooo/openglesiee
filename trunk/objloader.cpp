#include "objloader.h"

ObjLoader::ObjLoader(){
	m_file = NULL;
	m_obj = NULL;
}

ObjLoader::~ObjLoader(){
	freeObj();
}

ObjLoader::ObjLoader(std::string file){
	m_file = new std::string(file);
	m_mtlFile = new std::string();
	m_obj = new Object3D();
	printf("debut de lecture des donnes du fichier\n");
	readData();
	printf("fin de lecture\n");
}

void ObjLoader::loadObj(std::string file){
	freeObj();
	m_file = new std::string(file);
	m_mtlFile = new std::string();
	m_obj = new Object3D();
	readData();
}

void ObjLoader::freeObj(void){
	if(m_file != NULL)
		delete m_file;
	if(m_obj != NULL)
		delete m_obj;
}

Object3D& ObjLoader::returnObj(void){
	Object3D *ret = new Object3D(*m_obj);
	return *ret;
}

void ObjLoader::readData(void){
	std::ifstream input(m_file->c_str());
	std::string buffer;
	int cpt = 0;

	//verification de l'ouverture du fichier
	if( !input.is_open() )
		return;

	//passe pour stocker les donnees
	while( !input.eof() ){
		getline(input, buffer);
		std::istringstream line(buffer);
		std::string temp;
		std::string f1, f2, f3;
		vec3 temp_n, temp_v, temp_t;
		Triangle temp_f;
		cpt++;

		if(buffer.substr(0,2) == "vn"){
			line >> temp >> f1 >> f2 >> f3;
			temp_n.x = atof(f1.c_str());
			temp_n.y = atof(f2.c_str());
			temp_n.z = atof(f3.c_str());
			m_obj->m_normalArray.push_back(temp_n);
		}				
		else if(buffer.substr(0,2) == "vt"){
			line >> temp >> f1 >> f2;
			temp_t.x = atof(f1.c_str());
			temp_t.y = atof(f2.c_str());
			m_obj->m_texCoordArray.push_back(temp_t);
		}				
		else if(buffer.substr(0,1) == "v"){
			line >> temp >> f1 >> f2 >> f3;
			temp_v.x = atof(f1.c_str());
			temp_v.y = atof(f2.c_str());
			temp_v.z = atof(f3.c_str());
			m_obj->m_vertexArray.push_back(temp_v);
		}
		else if(buffer.substr(0,6) == "usemtl"){
			line >> temp >> f1;
			Material temp_m;
			temp_m.name = f1;
			m_obj->m_materialArray.push_back(temp_m);
            m_obj->m_materialArray[m_obj->m_materialArray.size()-1].firstVertice = cpt;
		}
		else if(buffer.substr(0,1) == "f"){
			line >> temp >> f1 >> f2 >> f3;
			
			int start = 0;
			int end = start;
			std::string temp;
			end = f1.find_first_of("/");
			if(end != (int)std::string::npos)
				read_face(0, start, end, f1, temp_f);

			start = 0;
			end = f2.find_first_of("/");
			if(end != (int)std::string::npos)
				read_face(1, start, end, f2, temp_f);

			start = 0;
			end = f3.find_first_of("/");
			if(end != (int)std::string::npos)
				read_face(2, start, end, f3, temp_f);

			m_obj->m_triangleArray.push_back(temp_f);
		}
		else if(buffer.substr(0,6) == "mtllib"){
			line >> temp >> f1;
            *m_mtlFile = f1;
		}
	}
}

void ObjLoader::read_face(int i, int start, int end, std::string &f, Triangle &temp_f)
{
	std::string temp;

	temp = f.substr(start, end - start);
	temp_f.Vertex[i] = atoi(temp.c_str()) - 1;

	start = end + 1;
	end = f.find("/", start+1);
	temp = f.substr(start, end - start);
	temp_f.TexCoord[i] = atoi(temp.c_str()) - 1;

	start = end + 1;
	end = f.length();
	temp = f.substr(start, end - start);
	temp_f.Normal[i] = atoi(temp.c_str()) - 1;
}

