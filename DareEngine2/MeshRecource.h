#pragma once
#include "glew.h"
#include <string>
#include <memory>
#include <vector>
#include "Vertex.h"

class MeshRecource{
public:
	MeshRecource(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	~MeshRecource();


	inline GLuint getVbo(){ return this->vbo; }
	inline GLuint getIbo(){ return this->ibo; }
	inline GLuint getVao(){ return this->vao; }
	

	inline int getSize(){ return this->size; }
	

	
	
	inline static std::shared_ptr<MeshRecource> makePtr(MeshRecource* meshRecource){ return std::shared_ptr<MeshRecource>(meshRecource); }
	
	inline std::vector<Vertex>& getVertices(){ return vertices; }

private:

	void fillBuffers(std::vector<unsigned int>& indices);

	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	
	std::vector<Vertex> vertices;
	

	unsigned int size;
};













