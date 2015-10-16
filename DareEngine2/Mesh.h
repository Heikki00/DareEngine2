#pragma once
#include <glew.h>
#include "Vertex.h"
#include <vector>
#include "FestusMath.h"
#include "DebugUtility.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include "MeshRecource.h"

#define NO_SRC_FILE "NO_SRC_FILE"

//Class that sends vertex data to gpu and has VAOs etc. 
class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string& name = NO_SRC_FILE);
	Mesh(std::string filename);
	~Mesh();





	void draw();

	
	inline static std::shared_ptr<Mesh> makePtr(Mesh* mesh){ return  std::shared_ptr<Mesh>(mesh); }

	inline std::vector<Vertex>& getVertices(){ return resource->getVertices(); }


private:
	
	
	void calcNorm(std::vector<Vertex>& vertices, std::vector<unsigned int>& indicies);

	std::shared_ptr<MeshRecource> resource;
	static std::unordered_map<std::string, std::shared_ptr<MeshRecource>> loadedModels;
	
	std::string filename;
	

	void loadMesh(std::string filename);

	
};

