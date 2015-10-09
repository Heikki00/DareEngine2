#include "ShaderRecource.h"


ShaderRecource::ShaderRecource()
{
	shader = glCreateProgram();
}


ShaderRecource::~ShaderRecource()
{
	glDeleteProgram(shader);

	for (auto stage : shaderStages){
		delete stage;
	}

}


inline ShaderStage* ShaderRecource::getShaderStage(GLenum type){
	for (auto stage : shaderStages){
		if (stage->getType() == type){
			return stage;
		}

	}
	
	return nullptr;

}


void ShaderRecource::addShaderStage(ShaderStage* stage){
	if (getShaderStage(stage->getType()) != nullptr){
		DebugUtility::log_err("ERROR: Tried to add shaderstage of type that was already stored to ShaderResource\n");
		return;

	}

	glAttachShader(shader, stage->getGLid());
	shaderStages.push_back(stage);


}


