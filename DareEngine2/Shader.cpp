#include "Shader.h"
#include "Time.h"
#include <algorithm>
#include "RenderingEngine.h"
#include <stdexcept>
#include <iostream>
#include "BaseLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "GameObject.h"
#include <fstream>
#include <sstream>
#include "ShaderStage.h"
#include "Uniform.h"
#include "GLSLParser.h"


std::unordered_map<std::string, std::shared_ptr<ShaderRecource>> Shader::loadedPrograms;

std::vector<UUCallback> Shader::staticCallbacks;

Shader::Shader(const std::string& filename): filename(filename){
	
	
	
	//If program is already loaded, just get the recource
	if (loadedPrograms.find(filename) != loadedPrograms.end()){
		resource = loadedPrograms[filename];
		
	}
	//Otherwise, load the program
	else{
		resource = ShaderRecource::makePtr();
		loadedPrograms[filename] = resource;
		
		//Loads the parser. If filename doesn't have an extension, add .glsl. Also, add path.
		const std::string filetype = ".glsl";
		const std::string path = "res/shaders/";

		if (filename.substr(filename.length() - filetype.length(), filetype.length()) != filetype) parser = new GLSLParser(path + filename + filetype);
		
		else parser = new GLSLParser(path + filename);

		loadShaders();

		link();

		loadUniforms();

	}


	

}


Shader::~Shader(){

	//If the recource is only referenced in this object and in loadedPrograms, erase it from loadedPrograms.
	//That way, it will get deleted with tis object
	if (loadedPrograms[filename].use_count() == 2){
		loadedPrograms.erase(filename);
		
	}

	delete parser;

	
}


void Shader::loadShaders(){
	
	
	resource->addShaderStage(new ShaderStage(parser->getShaderSource(GL_VERTEX_SHADER), GL_VERTEX_SHADER));
	
	resource->addShaderStage(new ShaderStage(parser->getShaderSource(GL_FRAGMENT_SHADER), GL_FRAGMENT_SHADER));


	if (!parser->getShaderSource(GL_GEOMETRY_SHADER).empty()) resource->addShaderStage(new ShaderStage(parser->getShaderSource(GL_GEOMETRY_SHADER), GL_GEOMETRY_SHADER));

	if (!parser->getShaderSource(GL_TESS_CONTROL_SHADER).empty()) resource->addShaderStage(new ShaderStage(parser->getShaderSource(GL_TESS_CONTROL_SHADER), GL_TESS_CONTROL_SHADER));
	
	if (!parser->getShaderSource(GL_TESS_EVALUATION_SHADER).empty()) resource->addShaderStage(new ShaderStage(parser->getShaderSource(GL_TESS_EVALUATION_SHADER), GL_TESS_EVALUATION_SHADER));
	

}







void Shader::bind(){
	
	if (!glIsProgram(resource->getShader()))
		DebugUtility::log_err("ERROR: Asked to bind invalid program: %s\n", filename.c_str());

	
	glUseProgram(resource->getShader());
	
}

void Shader::unbind(){
	glUseProgram(0);
	
}




void Shader::link()
{
	glLinkProgram(resource->getShader());
	DebugUtility::gl_check_program(resource->getShader());

	for (auto stage : resource->getShaderStages()){
		stage->attachToShader(this);
		glDeleteShader(stage->getGLid());
	}

	


}






void Shader::loadUniforms(){

	//How many active uniforms do we have?
	int amtUniforms;

	glGetProgramiv(resource->getShader(), GL_ACTIVE_UNIFORMS, &amtUniforms);

	//Iterate through them
	for (unsigned int i = 0; i < amtUniforms; ++i){
		
		//If uniform is a block, skip it
		int blockIndex = -1;
		glGetActiveUniformsiv(resource->getShader(), 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIndex);
		if (blockIndex != -1) continue;
		
		
		//Get uniform parameters
		char cname[100];
		GLsizei len;
		GLint size;
		GLenum etype;
		glGetActiveUniform(resource->getShader(), i, 1024, &len, &size, &etype, cname);

		std::string name(cname), type = Utilitys::enumToType(etype);

		//If uniforms name has a '.', a.k.a. if it is a struct.
		if (name.find_first_of('.') != name.npos){
			//Split name
			std::vector<std::string> splittedStruct = Utilitys::splitString(name, '.');
			
			std::string uniformBaseName = splittedStruct.at(0);

			std::string structType = parser->getUniformType(uniformBaseName);

			GLSLStruct structInfo = parser->getStruct(structType);


			//If we already have parts of struct loaded, set uniformStruct to that. If not, create new Uniform
			std::shared_ptr<Uniform> uniformStruct;
			for (auto& u : resource->uniforms){
				if (u->getName() == uniformBaseName){
					uniformStruct = u;
				}
			}
			
			if (uniformStruct == nullptr) uniformStruct = Uniform::makePtr(uniformBaseName, structType, this);
			
			//Iterate through uniforms splitted name, exluding base name
			auto currentyAddingTo = uniformStruct;
			auto currentlyAddingInfo = structInfo;

			for (int i = 1; i < splittedStruct.size(); ++i){
				//Name we are currently processing
				std::string currentName = splittedStruct.at(i);
				
				//If this uniform already exist, use that
				if (currentyAddingTo->hasChild(currentName)){
					currentyAddingTo = currentyAddingTo->getChild(currentName);
					
				
				}
				//If not, create new uniform
				else{
					currentyAddingTo->addChild(Uniform::makePtr(currentName, currentlyAddingInfo.getTypeOfChild(currentName), this));
					currentyAddingTo = currentyAddingTo->getChild(currentName);
				
				}
				//Get this uniforms info (GLSLStruct)
				currentlyAddingInfo = parser->getStruct(currentlyAddingInfo.getTypeOfChild(currentName));

			
			}

			//Add uniforms to resource
			resource->uniforms.push_back(uniformStruct);

		}
		//If uniform is not struct, just add it to recource
		else{
			resource->uniforms.push_back(Uniform::makePtr(name, type, this));
			
		}

	}

		




		//Load every uniforms location
		for (auto u : resource->uniforms) u->loadLocation();







	}






void Shader::updateUniforms(Transform* transform, Material* material, RenderingEngine* renderingEngine){

	//Make a vector of dynamic- + staticCallbacks
	std::vector<UUCallback> callbacks;
	callbacks.reserve(dynamicCallbacks.size() + staticCallbacks.size());
	callbacks.insert(callbacks.end(), staticCallbacks.begin(), staticCallbacks.end());
	callbacks.insert(callbacks.end(), dynamicCallbacks.begin(), dynamicCallbacks.end());

	
	
	//Get matrices
	Matrix4 worldMatrix = transform->getTransform();
	Matrix4 MVP = renderingEngine->getMainCamera()->getViewProjection() * worldMatrix;


	//Iterate through uniforms
	for (auto& uniformPointer : resource->uniforms){
		//Copy current uniform, so user can modify it without causing permanent changes
		Uniform uniform = *uniformPointer;


		//If we have any callbacks, iterate through them.If any returns true, skipt this uniform.
		if (callbacks.size() != 0){
			bool skip = false;

			for (auto call : callbacks){
				skip = call(transform, material, renderingEngine, *this, uniform, *uniformPointer) ? true : skip;

			}

			if (skip) continue;

		}

		
		//Uniforms name without the prefix. For example: R_directionalLight -> directionalLight
		std::string shortenUniformName = uniform.getName().substr(2, uniform.getName().length() - 2);

		//If starts with "T_" (Transform prefix)
		if (!strncmp(uniform.getName().c_str(), "T_", 2)){
			if (uniform.getName() == "T_MVP"){

				uniform.setValue(MVP);
			}
			else if (uniform.getName() == "T_model"){
				uniform.setValue(worldMatrix);
			}
			else{
				DebugUtility::log_err("ERROR: Invalid uniform with \"T_\" prefix found from shader %s:%s  %s\n", filename.c_str(), uniform.getType().c_str(), uniform.getName().c_str());
			}


		}


		//If starts with "R_" (RenderingEngine prefix)
		else if (!strncmp(uniform.getName().c_str(), "R_", 2)){


			if (shortenUniformName == "lightMatrix"){

				Matrix4 mat = renderingEngine->getLightMatrix() * worldMatrix;
				uniform.setValue(renderingEngine->getLightMatrix() * worldMatrix);

			}
			

			else if (uniform.getType() == "sampler2D"){

				std::string usableUniformName = shortenUniformName;

				int lenght = shortenUniformName.length();
				int targetTexture = 0;
				if (shortenUniformName.substr(lenght - 2, 1) == "_"){
					targetTexture = shortenUniformName.at(lenght - 1) - '0';

					usableUniformName = usableUniformName.substr(0, lenght - 2);
				}

				GLuint samplerSlot = renderingEngine->getSamplerSlot(shortenUniformName);


				renderingEngine->getTexture(usableUniformName)->bind(samplerSlot, targetTexture);

				uniform.setValue(samplerSlot);




			}

			else if (uniform.getType() == "vec3"){
				uniform.setValue(renderingEngine->getVector(shortenUniformName));
			}

			else if (uniform.getType() == "float"){
				uniform.setValue(renderingEngine->getFloat(shortenUniformName));
			}

			else if (uniform.getType() == "DirectionalLight"){
				uniform.setValue(*renderingEngine->getActiveLight());
			}

			else if (uniform.getType() == "PointLight"){
				uniform.setValue(*renderingEngine->getActiveLight());
			}

			else if (uniform.getType() == "SpotLight"){

				uniform.setValue(*renderingEngine->getActiveLight());
			}


			else{



			}
		}

		else if (!strncmp(uniform.getName().c_str(), "C_", 2)){
			if (uniform.getName() == "C_eyePos"){
				uniform.setValue(renderingEngine->getMainCamera()->getTransform()->getWorldPosition());
			}
			else{
				//DebugUtility::log_err("ERROR: Invalid uniform with \"C_\" prefix found from shader %s: %s %s\n", filename.c_str(), uniformType.c_str(), uniformName.c_str());
			}

		}


		else{
			if (uniform.getType() == "vec3"){

				uniform.setValue(material->getVector(uniform.getName()));
			}
			else if (uniform.getType() == "float"){
				
				uniform.setValue(material->getFloat(uniform.getName()));
			}


			if (uniform.getType() == "sampler2D"){

				GLuint samplerSlot = renderingEngine->getSamplerSlot(uniform.getName());

				material->getTexture(uniform.getName())->bind(samplerSlot);




				uniform.setValue((int)samplerSlot);
			}


		}

	

	}//LOOP END























}





















