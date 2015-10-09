#pragma once
#include "DareMath.h"

class Vertex
{
public:
	Vertex(const Vector3& position, const Vector3& texCoord = Vector3(0, 0, 0), const Vector3& normal = Vector3(0, 0, 0), const Vector3& tangent = Vector3(0, 0, 0));
	~Vertex();


	inline Vector3 getPos(){ return pos; }
	inline void setPos(const Vector3& pos){ this->pos = pos; }
	
	inline Vector3 getTexCoord() const { return texCoord; }
	inline void setTexCoord(const Vector3& val) { texCoord = val; }

	inline Vector3 getNormal() const { return normal; }
	inline void setNormal(const Vector3& val) { normal = val; }

	inline Vector3 getTanget() const { return tanget; }
	inline void setTanget(const Vector3& val) { tanget = val; }


private:
	Vector3 pos;
	Vector3 texCoord;
	Vector3 normal;
	Vector3 tanget;
	
};
