#include "GLSLParser.h"
#include <string>
#include <functional>


std::map<GLenum, std::string> GLSLParser::shaderTypeKeyMap;


GLSLParser::GLSLParser(const std::string& filename)
{
	this->filename = filename;
	src = Utilitys::readFile(filename);
	if (src.empty()){
		DebugUtility::log_err("ERROR: GLSLParser was given a file that didn't exist or wan empty: %s\n", filename.c_str());
		return;
	}

	addShaderTypeKeys();


	parseIncludes();
	parseInfo();

	parseStages();


}

std::string GLSLParser::getRawSrc()
{
	return Utilitys::readFile(filename);
}

std::string GLSLParser::getShaderSource(GLenum type)
{
	return shaderSources[type];
}





void GLSLParser::parseIncludes()
{
	
	//Recursive lambda function that Find and parses #includes and the files that it is going to include. Src is the current document to parse(look for #include:s), and includedFiles is a vector of filenames without
	//the extension that are already included, so kind of an auto-header-guard
	std::function<void (std::string&, std::vector<std::string>)> recursiveIncludeParser = [&](std::string& src, std::vector<std::string> includedFiles){
		const std::string INCLUDE_KEY = "#include";
		auto sPos = src.find(INCLUDE_KEY);

		
		while (sPos != std::string::npos){
			//get the whole line of #include first, then get the filename
			std::string line = src.substr(sPos, src.find_first_of('\n', sPos) - sPos);
			std::string includeFilename = "res/shaders/" + line.substr(line.find_first_of('\"') + 1, (line.find_last_of('\"') - 1) - (line.find_first_of('\"')));

			//if it isn't in includedFiles
			if (std::find(includedFiles.begin(), includedFiles.end(), includeFilename) == includedFiles.end()){

				includedFiles.push_back(includeFilename);

				//If it doesn't have extension, guess and see what opens.
				if (includeFilename.find_first_of('.') == std::string::npos){
					std::ifstream file;

					std::vector<std::string> fileExtensions = {
						"glsl", "glh", "verth", "fragh", "txt", "h", "hpp",
						"vert", "frag", "vs", "fs"

					};


					for (std::string& ext : fileExtensions){

						file.open(includeFilename + "." + ext);
						if (file.is_open()){
							includeFilename = includeFilename + "." + ext;




							break;
						}

					}

					file.close();



				}

				//Parse the included file and replace #include 'filename' with the content of filename
				std::string to = Utilitys::readFile(includeFilename);
				recursiveIncludeParser(to, includedFiles);

				src.replace(sPos, line.length(), to);




			}

			else{
				//If already included, just erase the line
				src.erase(sPos, line.length());

			}

			sPos = src.find(INCLUDE_KEY, sPos + 1);



		}

	};
	
	
	std::vector<std::string> included;
	
	recursiveIncludeParser(src, included);



}


void GLSLParser::parseInfo()
{
	
	//Find default version and set all shaders to it.

	const std::string VERSION_KEY = "#version";
	unsigned int vPos = src.find(VERSION_KEY);

	if (vPos == std::string::npos){
		DebugUtility::log_err("ERROR: No default shader version found in shader %s\n", filename.c_str());
	}

	unsigned int fNumPos = src.find_first_not_of(' ', vPos + VERSION_KEY.length());
	unsigned int lNumPos = src.find_first_of(' ', fNumPos) - 1;

	int defVersion = std::stoi(src.substr(fNumPos, lNumPos - fNumPos));


	shaderVersions[GL_VERTEX_SHADER] = defVersion;
	shaderVersions[GL_FRAGMENT_SHADER] = defVersion;
	shaderVersions[GL_GEOMETRY_SHADER] = defVersion;
	shaderVersions[GL_TESS_CONTROL_SHADER] = defVersion;
	shaderVersions[GL_TESS_EVALUATION_SHADER] = defVersion;


	//Find all structs

	const std::string STRUCT_KEY = "struct";
	unsigned int structPos = src.find(STRUCT_KEY);


	while (structPos != std::string::npos){
		
		//struct name parsing
		unsigned int structNameStart = src.find_first_not_of(' ', structPos + STRUCT_KEY.length());
		unsigned int structNameEnd = src.find_first_of(" {", structNameStart);

		std::string structName = src.substr(structNameStart, structNameEnd - structNameStart);

		//Find braces and create stringstream of text between braces. Loop throw stringstream and pray, that there is nothing else than variables in there.
		unsigned int fBracePos = src.find_first_of('{', structNameEnd);
		unsigned int lBracePos = src.find_first_of('}', fBracePos);

		std::stringstream structStream(src.substr(fBracePos + 1, (lBracePos - 1) - (fBracePos + 1)));

		

		GLSLStruct glslStruct(structName);
		
		while (structStream.good()){
			std::string type, name;
			
			structStream >> type >> name;
			
			//For the last, empty part of stream. No idea why it does that, but this solves it
			if (type.empty() || name.empty()) continue;

			//If ends to semicolon, remove it. If not, read it so we jump over it.
			if (name.at(name.length() - 1) == ';') name.erase(name.end() - 1);
			else structStream >> std::string();


			glslStruct.addChild(type, name);
		
		
		
		}


		structs.push_back(glslStruct);


		structPos = src.find(STRUCT_KEY, structPos + 1);

	
	
	
	}

	
	//find all uniforms in shader. Some of these might be optimized out when compiled, so still use glUniform. Mainly used with figurng out struct types of uniforms.
	//stores struct uniforms by their main struct. For example, "uniform DirectionalLight dir" stores as <dir, DirectionalLight> not as <float, dir.base.intensity>... etc.
	const std::string UNIFORM_KEY = "uniform";
	unsigned int uniPos = src.find(UNIFORM_KEY);

	while (uniPos != std::string::npos){
		unsigned int endPos = src.find_first_of(';', uniPos);

		std::stringstream  uniformStringStream(src.substr(uniPos + UNIFORM_KEY.length(), endPos - (uniPos + UNIFORM_KEY.length())));

		std::string name, type;

		uniformStringStream >> type >> name;

		uniforms[name] = type;
	
	
		uniPos = src.find(UNIFORM_KEY, uniPos + 1);
	
	
	}















}


void GLSLParser::parseStages()
{
	
	//Reusable lambda function to find stage of type and fill this->shaderSource with that. Returns true if found, false if not
	auto findAndParseStage = [&](GLenum type){

		
		if (src.find("START_" + shaderTypeKeyMap[type]) != std::string::npos){
			unsigned int sPos = src.find("START_" + shaderTypeKeyMap[type]);
			unsigned int startLineEnd = src.find_first_of('\n', sPos);

			unsigned int ePos = src.find("END_" + shaderTypeKeyMap[type]);
			unsigned int endLineEnd = src.find_first_of('\n', ePos);

			//Check if this has paragraphs and if they contain version for the shader
			if (src.find_first_of('(', sPos) < startLineEnd){
				std::string parameters = src.substr(src.find_first_of('(', sPos), src.find_first_of(')', sPos) - src.find_first_of('(', sPos));



				if(parameters.find("version") != std::string::npos) sscanf(parameters.c_str(), "(version=%i)", &shaderVersions[type]);



			}

			//add #version 'shader_version' to shader's source.
			std::string versionString = "#version " + std::to_string(shaderVersions[type]);


			shaderSources[type] = versionString + "\n" + src.substr(startLineEnd, ePos - startLineEnd);

		

			return true;
		}
		else return false;




	};




	if (!findAndParseStage(GL_VERTEX_SHADER)){
		DebugUtility::log_err("ERROR: No vertex shader found from shader %s\n", filename.c_str());
	}

	if (!findAndParseStage(GL_FRAGMENT_SHADER)){
		DebugUtility::log_err("ERROR: No fragment shader found from shader %s\n", filename.c_str());
	}

	findAndParseStage(GL_GEOMETRY_SHADER);
	findAndParseStage(GL_TESS_CONTROL_SHADER);
	findAndParseStage(GL_TESS_EVALUATION_SHADER);



}



void GLSLParser::addShaderTypeKeys(){
	if (shaderTypeKeyMap.find(GL_VERTEX_SHADER) != shaderTypeKeyMap.end()) return;

	shaderTypeKeyMap[GL_VERTEX_SHADER] = "VERTEX";
	shaderTypeKeyMap[GL_FRAGMENT_SHADER] = "FRAGMENT";
	shaderTypeKeyMap[GL_GEOMETRY_SHADER] = "GEOMETRY";
	shaderTypeKeyMap[GL_TESS_CONTROL_SHADER] = "TESS_CONTROL";
	shaderTypeKeyMap[GL_TESS_EVALUATION_SHADER] = "TESS_EVALUATION";
}


