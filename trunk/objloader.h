/*
Classe Objet3D
Author: Clement Robein

Classe de chargement des modèles 3D au format OBJ dans une classe Object3D

*/

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "object3d.h"

class ObjLoader  {
		public:
			ObjLoader();
			~ObjLoader();

			ObjLoader(std::string file);
			void loadObj(std::string file);
			void freeObj(void);
			Object3D& returnObj(void);

		protected:
			std::string *m_file;
			Object3D *m_obj;
			
			void readData(void);
	};

#endif
