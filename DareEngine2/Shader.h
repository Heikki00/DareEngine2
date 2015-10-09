#pragma once



#include "Utility.h"
#include <glew.h>
#include <vector>
#include "DebugUtility.h"
#include "DareMath.h"
#include <string>
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include <unordered_map>
#include "ShaderRecource.h"
#include "ShaderStage.h"
#include "GLSLParser.h"
#include "UUCallback.h"


class RenderingEngine;
class BaseLight;
class DirectionalLight;
class PointLight;
class SpotLight;
class ShaderStage;




class Shader
{
public:
	
	Shader(const std::string& filename);
	~Shader();
	
	
	



	
	//Tells opengl to use this program
	void bind();
	
	

	//Tells opengl not to use any shaders
	void unbind();

	




	//Updates programs uniforms. Model and perspective matrices from transform, lights and textures from RenderingEngine, and all the others from material.
	void updateUniforms(Transform* transform, Material* material, RenderingEngine* renderingEngine);

	

	inline GLint getGLid(){ return resource->getShader(); }


	inline static std::shared_ptr<Shader> makePtr(const std::string& filename){ return std::make_shared<Shader>(filename); }
	inline static std::shared_ptr<Shader> makePtr(Shader* shader){ return std::shared_ptr<Shader>(shader); }

	//Adds UUCallback, that gets called every time THIS shaders updateUniforms is called.
	inline void addDynamicCallback(UUCallback callback){ dynamicCallbacks.push_back(callback); }

	//Adds UUCallback, that gets called every time ANY shaders updateUniforms is called.
	static inline void addStaticCallback(UUCallback callback){ staticCallbacks.push_back(callback); }

	inline void removeDynamicCallback(UUCallback callback){ for (auto i = dynamicCallbacks.begin(); i != dynamicCallbacks.end(); ++i) if (*i == callback) dynamicCallbacks.erase(i); }
	static inline void removeStaticCallback(UUCallback callback){ for (auto i = staticCallbacks.begin(); i != staticCallbacks.end(); ++i) if (*i == callback) staticCallbacks.erase(i); }

	//returns filename given to constructor. Might not have extension
	inline std::string getFilename() const{ return filename; }


private:
	
	//Stored form constructor, not modified. Used to identify shader in debugging
	const std::string filename;

	//Loads shader sources from parser and adds ShaderStages to recource
	void loadShaders();

	//Links the shader
	void link();

	//Loads uniforms by iterating through active uniforms. Adds uniforms to recource. Also loads locations of uniforms, so call after linking the shader
	void loadUniforms();

	//Programs own recource, might be taken from loadedPrograms
	std::shared_ptr<ShaderRecource> resource;
	
	//When program is loaded, its data will be stored to this map. If program has already been loaded before, we can just take that data from here.
	static std::unordered_map<std::string, std::shared_ptr<ShaderRecource>> loadedPrograms;


	GLSLParser* parser;
	
	//Local callbacks
	std::vector<UUCallback> dynamicCallbacks;
	//Global callbacks
	static std::vector<UUCallback> staticCallbacks;

	
};

