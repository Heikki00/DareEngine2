#include "MeshRecource.h"
#include "DebugUtility.h"
#include "FestusMath.h"

MeshRecource::MeshRecource(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);
	this->vertices = vertices;
	
	
	this->size = indices.size();

	fillBuffers(indices);


}



MeshRecource::~MeshRecource(){
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}


void MeshRecource::fillBuffers(std::vector<unsigned int>& indices){

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int vector3dataSize = sizeof(float) * 3;
	int vertexDataSize = vector3dataSize * 4;


	float* vertexData = new float[vertices.size() * 12];


	const int amtData = 3 * 4;
	for (int i = 0; i < vertices.size(); ++i){
		
		memcpy(vertexData + (i * amtData), vertices.at(i).getPos().toArray(), vector3dataSize);
		memcpy(vertexData + (i * amtData) + 3, vertices.at(i).getTexCoord().toArray(), vector3dataSize);

		memcpy(vertexData + (i * amtData) + 6, vertices.at(i).getNormal().toArray(), vector3dataSize);

		memcpy(vertexData + (i * amtData) + 9, vertices.at(i).getTanget().toArray(), vector3dataSize);

	}


	glBufferData(GL_ARRAY_BUFFER,  vertices.size() * vertexDataSize, vertexData, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(vector3dataSize));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(vector3dataSize * 2));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(vector3dataSize * 3));
	glEnableVertexAttribArray(3);




	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);












}



