#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model(char *path)
	{
		loadModel(path); 
	}

private:
	std::vector<Mesh> meshes;
	std::string directory; 

	void loadModel(std::string path);
	void processNode(); 


};

#endif 

