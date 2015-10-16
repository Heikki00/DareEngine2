#include "ShaderStage.h"
#include "Shader.h"
#include <algorithm>
#include "DebugUtility.h"
#include "FestusMath.h"
#include "Material.h"
#include "Transform.h"
#include "RenderingEngine.h"
#include <vector>


typedef std::pair<std::string, std::string> strPair;

ShaderStage::ShaderStage(std::string src, GLenum type){
	this->src = src;
	this->type = type;
	
	parseInfo();

	

	createGLShader();


}





void ShaderStage::parseInfo(){
	const std::string VERSION_KEY = "#version";
	int pos = src.find(VERSION_KEY);
	if (pos == src.npos){
		version = 140;
	}
	else{
		std::string line = src.substr(pos, src.find_first_of('\n', pos) - pos);
		line = line.substr(VERSION_KEY.length(), line.length() - VERSION_KEY.length());
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		version = std::stoi(line);
	}


}


void ShaderStage::createGLShader(){
	GLuint sh = glCreateShader(type);

	const GLchar* srcStrings[1];
	GLint srcStringLenght[1];

	srcStrings[0] = src.c_str();
	srcStringLenght[0] = src.length();

	glShaderSource(sh, 1, srcStrings, srcStringLenght);
	glCompileShader(sh);
	
	if (DebugUtility::gl_check_shader(sh))
		DebugUtility::log("Successfully compiled shader %i\n", sh);
	else
		DebugUtility::log_err("ERROR: Failed to chech shader %i\n", sh);


	GLid = sh;

	



}


void ShaderStage::attachToShader(Shader* shader){
	parents.push_back(shader);
	++useCount;

	
	
	if (shader->getFilename() != "testShader") return;


	int amt;

	glGetProgramiv(shader->getGLid(), GL_ACTIVE_UNIFORMS, &amt);

	


	
	}

	
	


































































