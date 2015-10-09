#define _CRT_SECURE_NO_WARNINGS
#include "DebugUtility.h"
#include <iostream>


bool DebugUtility::check_opengl_error(){

	const int numParanms = 6;

	std::string sEnums[numParanms]{
		"GL_NO_ERROR",
			"GL_INVALID_ENUM",
			"GL_INVALID_VALUE",
			"GL_INVALID_OPERATION",
			"GL_INVALID_FRAMEBUFFER_OPERATION",
			"GL_OUT_OF_MEMORY"
	};


	GLenum enums[numParanms]{
		GL_NO_ERROR,
			GL_INVALID_ENUM,
			GL_INVALID_VALUE,
			GL_INVALID_OPERATION,
			GL_INVALID_FRAMEBUFFER_OPERATION,
			GL_OUT_OF_MEMORY

	};

	GLenum code = glGetError();


	if (code == GL_NO_ERROR){
		
		
		return true;

	}

	std::string errorMsg = "ERROR_UNDEFINED";
	for (int i = 0; i < numParanms; i++){
		if (code == enums[i]){
			errorMsg = sEnums[i];
			break;
		}
	}
	

	

	log_err("ERROR: Caught OpenGL Error Message: %s\n", errorMsg.c_str());
	//gl_log_err("ERROR: Caught OpenGL Error Message: %i\n", code);

	return false;

}






bool DebugUtility::restart_log(){
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file){
		fprintf(stderr, "ERROR: could not open gl log file: %s for writing\n", GL_LOG_FILE);
		return 0;
	}
	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf(file, "GL_LOG_FILE log. Local time: %s\n", date);
	fclose(file);
	return 1;

}


bool DebugUtility::log(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
			);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}


bool DebugUtility::log_err(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
			);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}


void DebugUtility::gl_log_params() {
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	DebugUtility::log("GL Context Params:\n");
	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		DebugUtility::log("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	DebugUtility::log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	DebugUtility::log("%s %u\n", names[11], (unsigned int)s);
	DebugUtility::log("-----------------------------\n");
}


void DebugUtility::gl_print_shader_info_log(GLuint shader_index) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
	printf("shader info log for GL index %u:\n%s\n", shader_index, log);
}



bool DebugUtility::gl_check_shader(GLuint shader_index){

	int params = -1;
	glGetShaderiv(shader_index, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params){
		DebugUtility::log_err("ERROR: Shader %i did not compile correctly\n", shader_index);
		DebugUtility::gl_print_shader_info_log(shader_index);
		return false;


	}
	return true;
}



void DebugUtility::gl_print_program_info_log(GLuint programm) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog(programm, max_length, &actual_length, log);
	printf("Program info log for GL index %u:\n%s", programm, log);
}

bool DebugUtility::gl_check_program(GLuint program){
	int params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
		//I'm sorry please don't kill me!
		goto error;

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	if (GL_TRUE != params)
		goto error;

	return true;

error:
	DebugUtility::log_err("Program %i is not valid nor did it link properly\n", program);
	DebugUtility::gl_print_program_info_log(program);
	return false;
}









