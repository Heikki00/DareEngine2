#include "MappedValues.h"
#include "DebugUtility.h"

void MappedValues::setVector(const std::string& name, const Vector3& value){
	vectors[name] = value;
}

MappedValues::~MappedValues(){
	for (auto& ptr : pointers){
		
		if (ptr.second) delete ptr.second;
	}

}



void MappedValues::setFloat(const std::string& name, float value){
	floats[name] = value;
}


void MappedValues::setTexture(const std::string& name, std::shared_ptr<Texture> value){
	textures[name] = value;
}

void MappedValues::setPointer(const std::string& name, void* value){

	pointers[name] = value;

}


Vector3 MappedValues::getVector(const std::string& name){

	if (vectors.find(name) != vectors.end()){
		return vectors[name];
	}

	DebugUtility::log("Could not find vector with name %s from MappedValues\n", name.c_str());
	return Vector3(0, 0, 0);
}




float MappedValues::getFloat(const std::string& name){

	if (floats.find(name) != floats.end()){
		return floats[name];
	}



	DebugUtility::log("Could not find float with name %s from MappedValues\n", name.c_str());
	return 0.0f;

}



std::shared_ptr<Texture> MappedValues::getTexture(const std::string& name){

	if (textures.find(name) != textures.end()){
		return textures[name];
	}



	DebugUtility::log("Could not find texture with name %s from MappedValues\n", name.c_str());
	return nullptr;

}




void* MappedValues::getPointer(const std::string& name){

	if (pointers.find(name) != pointers.end()){
		return pointers[name];
	}



	DebugUtility::log("Could not find pointer with name %s from MappedValues\n", name.c_str());
	return nullptr;

}













