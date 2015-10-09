#pragma once
#include <functional>
#include <string>

class Transform;
class Material;
class RenderingEngine;
class Shader;
class Uniform;

class UUCallback{
public:
	typedef std::function<bool(Transform* transform, Material* material, RenderingEngine* renderingEngine, const Shader& shader, Uniform& localUniform, Uniform& storedUniform)> UUCallBackFunction;


	UUCallback(UUCallBackFunction callback){
		this->callback = callback;
		localIndex = index;
		++index;

	}



	inline bool operator ==(const UUCallback& other){
		return localIndex == other.localIndex;
	}
	inline bool operator <(const UUCallback& other){
		return localIndex < other.localIndex;
	}

	inline bool operator >(const UUCallback& other){
		return localIndex > other.localIndex;
	}

	inline bool operator ()(Transform* transform, Material* material, RenderingEngine* renderingEngine, const Shader& shader, Uniform& localUniform, Uniform& storedUniform){
		return callback(transform, material, renderingEngine, shader, localUniform, storedUniform);
	}
	
	inline UUCallBackFunction getCallback(){ return callback; }

private:

	UUCallBackFunction callback;

	unsigned long localIndex;
	static unsigned long index;




};


