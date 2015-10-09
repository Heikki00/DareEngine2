#include "Uniform.h"
#include "Shader.h"
#include "ShaderStage.h"
#include "BaseLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


#pragma region Uniform

std::map < std::string, std::function<void(void*, Uniform&)> > Uniform::setCallbacks;



Uniform::Uniform(const std::string& name, const std::string& type, Shader* shader) : type(type){
	Uniform::loadDefaultCallbacks();

	this->name = name;
	location = -1u;
	this->shader = shader->getGLid();
	parent = nullptr;

	
	



}


Uniform::~Uniform(){
	
}



void Uniform::loadLocation(){

	if (glIsProgram(shader) == GL_FALSE){
		DebugUtility::log_err("ERROR: Called loadLocation of uniform %s before its shader was successfully linked\n", name.c_str());
		return;
	}

	if (!isStruct()){

		
		std::string realName = getRealName();
		

		location = glGetUniformLocation(shader, realName.c_str());


		if (location == -1u) {
			DebugUtility::log_err("ERROR: Failed to load location of uniform %s\n", realName.c_str());
			

		}
		else{
			
		}


	}

	else {
	
		for (auto c : children){
			c->loadLocation();
		}


	}

}






GLuint Uniform::getLocation(){
	return location;

}



void Uniform::loadDefaultCallbacks()
{
	if (setCallbacks["int"] != nullptr) return;
	

	setCallbacks["int"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<int*>(val));

	};

	setCallbacks["uint"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<unsigned int*>(val));
	};

	setCallbacks["float"] = [](void* val, Uniform& u){
		glUniform1f(u.getLocation(), *static_cast<float*>(val));
	
	};

	setCallbacks["double"] = [](void* val, Uniform& u){
		glUniform1d(u.getLocation(), *static_cast<double*>(val));

	};

	setCallbacks["bool"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<bool*>(val));
	};

	setCallbacks["sampler1D"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<int*>(val));
	};

	setCallbacks["sampler2D"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<int*>(val));
	};

	setCallbacks["sampler3D"] = [](void* val, Uniform& u){
		glUniform1i(u.getLocation(), *static_cast<int*>(val));
	};

	setCallbacks["vec2"] = [](void* val, Uniform& u){
		glUniform2f(u.getLocation(), static_cast<Vector2*>(val)->x, static_cast<Vector2*>(val)->y);
	};

	setCallbacks["vec3"] = [](void* val, Uniform& u){
		Vector3* v = static_cast<Vector3*>(val);
		glUniform3f(u.getLocation(), v->x, v->y, v->z);
	};

	setCallbacks["vec4"] = [](void* val, Uniform& u){
		glUniform4f(u.getLocation(), static_cast<Vector4*>(val)->x, static_cast<Vector4*>(val)->y, static_cast<Vector4*>(val)->z, static_cast<Vector4*>(val)->w);
	};

	setCallbacks["mat4"] = [](void* val, Uniform& u){
		
		//std::cout << v << std::endl;

		glUniformMatrix4fv(u.getLocation(), 1, GL_TRUE, static_cast<Matrix4*>(val)->toArray());
	};


	setCallbacks["BaseLight"] = [](void* val, Uniform& u){
		BaseLight* base = static_cast<BaseLight*>(val);
		
		
		if (u.hasChild("intensity")) u.getChild("intensity")->setValue(base->getIntensity());
		if (u.hasChild("color")) u.getChild("color")->setValue(&base->getColor());


	};

	setCallbacks["DirectionalLight"] = [](void* val, Uniform& u){
		DirectionalLight* dir = static_cast<DirectionalLight*>(val);
		
		if (u.hasChild("base")) u.getChild("base")->setValue(static_cast<BaseLight*>(dir));
		if (u.hasChild("direction")) u.getChild("direction")->setValue(dir->getDirection());


	};

	setCallbacks["PointLight"] = [](void* val, Uniform& u){
		PointLight* poi = static_cast<PointLight*>(val);
		
		

		if (u.hasChild("base")) u.getChild("base")->setValue(static_cast<BaseLight*>(poi));
		
		if (u.hasChild("constant")) u.getChild("constant")->setValue(poi->getConstant());
		if (u.hasChild("linear")) u.getChild("linear")->setValue(poi->getLinear());
		if (u.hasChild("exponent")) u.getChild("exponent")->setValue(poi->getExponent());

		if (u.hasChild("position")) u.getChild("position")->setValue(poi->getTransform()->getWorldPosition());
		if (u.hasChild("range")) u.getChild("range")->setValue(poi->getRange());

	};

	setCallbacks["SpotLight"] = [](void* val, Uniform& u){
		SpotLight* spot = static_cast<SpotLight*>(val);
		
		
		if (u.hasChild("pointLight")) u.getChild("pointLight")->setValue(static_cast<PointLight*>(spot));
		
		if (u.hasChild("direction")) u.getChild("direction")->setValue(&spot->getDirection());

		if (u.hasChild("cutoff")) u.getChild("cutoff")->setValue(spot->getCutoff());
		
	};



}

std::string Uniform::getRealName()
{
	if (parent == nullptr) return name;

	return parent->getRealName() + "." + name;

}




#pragma endregion Uniform






