#include "Utility.h"


std::string Utilitys::readFile(const std::string& filename){
	
	std::ifstream is(filename, std::ios::binary);
	
	if (!is){
		DebugUtility::log_err("ERROR: Failed to open a text file for reading: %s\n", filename.c_str());
		return "";
	}
	
	is.seekg(0, std::ios_base::end);
	
	
	std::string result;

	result.reserve(is.tellg());
	is.seekg(0, std::ios_base::beg);

	result.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

	
	is.close();

	return result;
}



std::stringstream Utilitys::readFileToStream(const std::string& filename){
	std::ifstream is(filename, std::ios::binary);

	if (!is){
		DebugUtility::log_err("ERROR: Failed to open a text file for reading: %s\n", filename.c_str());
		return std::stringstream();
	}

	
	std::stringstream result;
	result << is.rdbuf();
	

	is.close();

	return result;


}


unsigned int Utilitys::uniformTypeToSize(const std::string& type){
	if (type == "int") return sizeof(int);
	if (type == "uint") return sizeof(unsigned int);
	if (type == "float") return sizeof(float);
	if (type == "double") return sizeof(double);
	if (type == "bool") return sizeof(bool);
	if (type == "char") return sizeof(char);
	if (type == "vec4") return sizeof(Vector3);
	if (type == "vec3") return sizeof(Vector3);
	if (type == "vec2") return sizeof(Vector2);
	if (type == "mat4") return sizeof(Matrix4);
	if (type == "mat3") return sizeof(Matrix3);
	if (type == "sampler3D") return sizeof(int);
	if (type == "sampler2D") return sizeof(int);
	if (type == "sampler1D") return sizeof(int);
	return 0;
}



std::vector<std::string> Utilitys::splitString(const std::string& src, char delimiter){
	size_t start = 0;
	size_t end = src.find_first_of(delimiter);

	std::vector<std::string> output;

	while (end <= std::string::npos)
	{
		output.emplace_back(src.substr(start, end - start));

		if (end == std::string::npos)
			break;

		start = end + 1;
		end = src.find_first_of(delimiter, start);
	}

	for (unsigned int i = 0; i < output.size(); i++){
		if (output.at(i) == ""){
			output.erase(output.begin() + i);
		}
	}


	return output;

}

std::string Utilitys::enumToType(GLenum typeEnum){

	switch (typeEnum){
	
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_DOUBLE: return "double";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_BOOL: return "bool";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_CUBE: return "vec4";
	default: return "";
	
	
	
	
	
	
	
	}






}





