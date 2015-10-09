#include "Vertex.h"




Vertex::Vertex(const Vector3& position, const Vector3& texCoord, const Vector3& normal, const Vector3& tanget){
	this->pos = position;
	this->texCoord = texCoord;
	this->normal = normal;
	this->tanget = tanget;
	
}


Vertex::~Vertex()
{
}
