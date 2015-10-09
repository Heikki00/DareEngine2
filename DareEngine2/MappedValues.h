#pragma once
#include <unordered_map>
#include <string>
#include "DareMath.h"
#include <memory>

class Texture;

class MappedValues{
public:

	~MappedValues();

	void setVector(const std::string& name, const Vector3& value);
	void setFloat(const std::string& name, float value);
	void setTexture(const std::string& name, std::shared_ptr<Texture> value);
	void setPointer(const std::string& name, void* value);

	Vector3 getVector(const std::string& name);
	float getFloat(const std::string& name);
	std::shared_ptr<Texture> getTexture(const std::string& name);
	void* getPointer(const std::string& name);

private:
	std::unordered_map<std::string, Vector3> vectors;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	std::unordered_map<std::string, void*> pointers;
};




