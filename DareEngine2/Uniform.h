#pragma once

#include <string>
#include <unordered_map>
#include "glew.h"
#include "DareMath.h"
#include <memory>
#include "Texture.h"
#include <functional>
#include <vector>
#include <map>

class Shader;

class BaseLight;
class DirectionalLight;
class PointLight;
class SpotLight;



class Uniform {
public:
	
	Uniform(const std::string& name, const std::string& type, Shader* shader);

	~Uniform();

	
	inline std::string getName() const{ return name; }
	inline void setName(const std::string& name){ this->name = name; }
	inline std::string getType() const{ return type; }

	//Returns location of the uniform in shader. If loadLocation failed or has not been called yet, returns -1u
	GLuint getLocation();

	//Loads location of the uniforms with glGetUniformLocation. Excepts that shader is successfully linked. If not, location is not loaded and error message is sent.'
	//If uniform is struct, calls loadLocation() of all its children
	void loadLocation();

	//Sets the value of uniform. This overload is for non-pointer types
	template<typename T>
	void setValue(T value){
		setValue(&value);
		

	}
	
	//Sets the value of uniform. This overload is for pointer types. Works just like above.
	template<typename T>
	void setValue(T* value){
		
		setCallbacks[type](static_cast<void*>(value), *this);
		



	}
	


	//name < other.name
	inline bool operator <(const Uniform& other){
		return name < other.name;

	}

	//Adds lambda for type. When uniform is set, uniform calls lambda that is acocciated with its type. Callbacks are static. Default callbacks are provided for basic light and GLSL types.
	//Use this to set custom structs.
	inline void addCallback(const std::string& type, std::function<void(void*, Uniform&)> func){ setCallbacks[type] = func; }


	friend std::ostream& operator <<(std::ostream& os, const Uniform& u){
		os << u.getName();
		
		
		return os;


	}

	inline void addChild(std::shared_ptr<Uniform> child){ children.push_back(child); child->parent = this; }
	inline void removeChild(std::shared_ptr<Uniform> child){ for (auto i = children.begin(); i != children.end(); ++i) if (*i = child) children.erase(i); child->parent = nullptr; }
	inline std::shared_ptr<Uniform> getChild(const std::string& name){ for (auto i = children.begin(); i != children.end(); ++i) if ((*i)->getName() == name) return *i; return nullptr; }
	inline std::vector<std::shared_ptr<Uniform>> getChildren(){ return children; }
	inline int numChild() const{ return children.size(); }
	inline bool hasChild(const std::string& name){ for (auto i = children.begin(); i != children.end(); ++i) if ((*i)->getName() == name) return true; return false; }

	static inline std::shared_ptr<Uniform> makePtr(Uniform* u){ return std::shared_ptr<Uniform>(u); }
	static inline std::shared_ptr<Uniform> makePtr(const std::string& name, const std::string& type, Shader* shader){ return std::make_shared<Uniform>(name, type, shader); }

	//returns false if getType() is any of basic GLSL types: int, uint, vec2, mat4 etc.
	inline bool isStruct(){
		return type != "int" && type != "float" && type != "double" &&
			type != "uint" && type != "vec2" && type != "vec3" &&
			type != "vec4" && type != "mat2" && type != "mat3" &&
			type != "mat4" && type != "sampler2D" && type != "sampler3D" &&
			type != "sampler1D" && type != "bool";
	}

private:

	//location of uniform. default -1u
	GLuint location;

	//Parent of uniform. Parent is set when uniform id added with addChild. can be nullptr
	Uniform* parent;


	
	//Type of uniform: int, float, mat4, DirectionalLight etc.
	const std::string type;

	//Name of the uniform
	std::string name;


	GLuint shader;


	std::vector<std::shared_ptr<Uniform>> children;

	//Returns name for uniforms that don't have parent. for other, returns the whole name, for example: directionalLight.base.intensity
	std::string getRealName();

	//Stores callback lambdas to set uniforms
	static std::map < std::string, std::function<void(void*, Uniform&)> > setCallbacks;
	
	//loads default callbacks, if not loaded already
	static void loadDefaultCallbacks();

};


class GLSLStruct{
public:

	GLSLStruct(const std::string& type){
		this->type = type;
		
	}

	void addChild(const std::string& type, const std::string& name){
		children.push_back(std::pair<std::string, std::string>(type, name));
	}

	std::string getTypeOfChild(const std::string& name){ for (auto& p : children) if (name == p.second) return p.first; return ""; }

	std::string type;

	std::vector<std::pair<std::string, std::string>> children;
	
};






