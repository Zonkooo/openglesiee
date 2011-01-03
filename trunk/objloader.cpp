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
	m_obj = new Object3D();
	printf("debut de lecture des donnes du fichier\n");
	readData();
	printf("fin de lecture\n");
}

void ObjLoader::loadObj(std::string file){
	freeObj();
	m_file = new std::string(file);
	m_obj = new Object3D();
	readData();
}

void ObjLoader::freeObj(void){
	if(m_file != NULL)
		delete m_file;
	if(m_obj != NULL)
		delete m_obj;
}

Object3D ObjLoader::returnObj(void){
	Object3D ret(*m_obj);
	return ret;
}

void ObjLoader::readData(void){
	std::ifstream input(m_file->c_str());
	std::string buffer;

	//verification de l'ouverture du fichier
	if( !input.is_open() )
		return;

	//creation des index
	int i_n, i_v, i_t, i_f;
	i_n = i_v = i_t = i_f = 0;

	//deuxieme passe pour stocker les donnees
	while( !input.eof() ){
		getline(input, buffer);
		std::istringstream line(buffer);
		std::string temp;
		std::string f1, f2, f3;
		vec3 temp_n, temp_v, temp_t;
		Triangle temp_f;

		if(buffer.substr(0,2) == "vn"){
			line >> temp >> f1 >> f2 >> f3;
			temp_n.x = atof(f1.c_str());
			temp_n.y = atof(f2.c_str());
			temp_n.z = atof(f3.c_str());
			m_obj->m_normalArray.push_back(temp_n);
			
			//printf("lecture vn\n");

			i_n++;
		}				
		else if(buffer.substr(0,2) == "vt"){
			line >> temp >> f1 >> f2;
			temp_t.x = atof(f1.c_str());
			temp_t.y = atof(f2.c_str());
			m_obj->m_texCoordArray.push_back(temp_t);

			//printf("lecture vt\n");

			i_t++;
		}				
		else if(buffer.substr(0,1) == "v"){
			line >> temp >> f1 >> f2 >> f3;
			temp_v.x = atof(f1.c_str());
			temp_v.y = atof(f2.c_str());
			temp_v.z = atof(f3.c_str());
			m_obj->m_vertexArray.push_back(temp_v);

			//printf("lecture v\n");

			i_v++;
		}
		else if(buffer.substr(0,1) == "f"){
			line >> temp >> f1 >> f2 >> f3;
			
			int start = 0;
			int end = start;
			std::string temp;
			end = f1.find_first_of("/");
			//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
			if(end != std::string::npos){
				temp = f1.substr(start, end - start);
				temp_f.Vertex[0] = atoi(temp.c_str()) - 1;

				start = end+1;
				end = f1.find("/", start);
				temp = f1.substr(start, end - start);
				temp_f.Vertex[1] = atoi(temp.c_str()) - 1;

				start = end+1;
				end = f1.length();
				temp = f1.substr(start, end - start);
				temp_f.Vertex[2] = atoi(temp.c_str()) - 1;
			}

			start = 0;
			end = f2.find_first_of("/");
			//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
			if(end != std::string::npos)  {
				temp = f2.substr(start, end - start);
				temp_f.TexCoord[0] = atoi(temp.c_str()) - 1;

				start = end + 1;
				end = f2.find("/", start+1);
				temp = f2.substr(start, end - start);
				temp_f.TexCoord[1] = atoi(temp.c_str()) - 1;

				start = end + 1;
				end = f2.length();
				temp = f2.substr(start, end - start);
				temp_f.TexCoord[2] = atoi(temp.c_str()) - 1;
			}

			start = 0;
			end = f3.find_first_of("/");
			//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
			if(end != std::string::npos)  {
				temp = f3.substr(start, end - start);
				temp_f.Normal[0] = atoi(temp.c_str()) - 1;

				start = end + 1;
				end = f3.find("/", start+1);
				temp = f3.substr(start, end - start);
				temp_f.Normal[1] = atoi(temp.c_str()) - 1;

				start = end + 1;
				end = f3.length();
				temp = f3.substr(start, end - start);
				temp_f.Normal[2] = atoi(temp.c_str()) - 1;
			}

			m_obj->m_triangleArray.push_back(temp_f);

			//printf("lecture f\n");

			i_f++;
		}
	}
}
