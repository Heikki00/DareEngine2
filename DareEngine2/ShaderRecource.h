#pragma once
#include <glew.h>
#include <memory>
#include "Utility.h"
#include <unordered_map>
#include "ShaderStage.h"
#include "Uniform.h"

class ShaderRecource
{
public:
	ShaderRecource();
	~ShaderRecource();

	inline GLuint getShader() const{ return shader; }

	inline static std::shared_ptr<ShaderRecource> makePtr(){ return std::make_shared<ShaderRecource>(); }

	inline std::vector<ShaderStage*> getShaderStages() { return shaderStages; }
	void addShaderStage(ShaderStage* stage);
	ShaderStage* getShaderStage(GLenum type);
	
	std::vector<std::shared_ptr<Uniform>> uniforms;



private:

	std::vector<ShaderStage*> shaderStages;
	GLuint shader;

};

