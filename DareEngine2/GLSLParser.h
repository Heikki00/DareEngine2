#pragma once
#include <string>
#include <vector>
#include "Uniform.h"
#include "glew.h"
#include <map>

class GLSLParser{
public:

	//Constructor reads and parses the file. Fi file is empty or does no exist, prints error message and does not parse;
	GLSLParser(const std::string& filename);

	//Returns specified file parsed. For example, #includes are parsed but VERTEX_START is still in file
	inline std::string getParsedSrc() const{ return src; }

	//Returns content of specified file. Reads it again.
	std::string getRawSrc();

	//Returns clean GLSL code for the shader specified. If shader of 'type' does no exist, returns empty string. Starts with #version 'shader_version'
	std::string getShaderSource(GLenum type);

	//Returns GLSL version of 'type'. If no specific version was specified in paragraphs after START_'type', returns default version from the start of the file(more common)
	inline int getVersion(GLenum type) { return shaderVersions[type]; }
	
	//returns GLSLStruct representation of struct of 'type'. If struct 'type' was not found, returns empty GLSLStruct.
	inline GLSLStruct getStruct(const std::string& type) const{ for (auto& s : structs) if (s.type == type) return s; return GLSLStruct(""); }
	
	//Returns vector of all structs found.
	inline std::vector<GLSLStruct> getStructs() const{ return structs; }

	//Returns type of uniform 'name'. If uniform 'name' was not defined, returns empty string
	inline std::string getUniformType(const std::string& name) { return uniforms[name]; }

	//returns map <uniform_name, uniform_type> of all uniforms found(Some may be optimized out when shader is compiled)
	inline std::map<std::string, std::string> getUniforms() const{ return uniforms; }
	


private:
	
	std::string src;
	std::string filename;
	
	std::vector<GLSLStruct> structs;

	//maps shader types to clean GLSL sources(#version included)
	std::map<GLenum, std::string> shaderSources;

	//Version of each shader. If not specified, it is the default version(defined with #version ''). Defined with START_VERTEX(version=100). No spaces allowed
	
	std::map<GLenum, int> shaderVersions;
	
	//Mapping uniform names to uniform types
	std::map<std::string, std::string> uniforms;

	//Replaces #include "filename":s with contents of filename. Does not include same file twice.
	void parseIncludes();
	
	//Reads info from the file. Versions, structs, uniforms etc. Sets shaderVersions to default versions
	void parseInfo();
	
	//Fills shaderSources
	void parseStages();

	//For example <GL_VERTEX_SHADER, VERTEX> or <GL_GEOMETRY_SHADER, GEOMETRY>. Used to wind START_:s and END_:s of shaders
	static std::map<GLenum, std::string> shaderTypeKeyMap;

	//Fills shaderTypeKeyMap if not filled. If filled, just exits.
	void addShaderTypeKeys();

};