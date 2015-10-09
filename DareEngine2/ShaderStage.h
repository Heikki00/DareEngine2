#pragma once
#include <string>
#include <glew.h>
#include <vector>
#include <unordered_map>
#include <condition_variable>
#include "Uniform.h"

class Shader;
class Transform;
class RenderingEngine;
class Material;


class ShaderStage{
public:
	
	

	ShaderStage(std::string src, GLenum type);
	
	~ShaderStage(){};


	void attachToShader(Shader* shader);

	
	inline GLenum getType() const{ return type; }
	inline int getUseCount() const{ return useCount; }
	inline int getVersion() const{ return version; }
	inline GLuint getGLid() { return GLid; }

private:
	
	
	int version;
	int useCount;
	GLenum type;
	GLuint GLid;

	std::vector<Shader*> parents;
	
	
	std::string src;

	


	void parseInfo();
	void createGLShader();
	


};
