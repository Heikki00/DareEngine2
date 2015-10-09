#pragma once
#include <glew.h>
#include <time.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#define GL_LOG_FILE "engine.log"
namespace DebugUtility{


	//Opens file and writes local time to it. Called when starting application(hopefully...)
	bool restart_log();
	
	//Writes stuff in log file
	bool log(const char* message, ...);
	
	//Writes stuff in log file, AND prints it on the console. Used for error messages(duh...)
	bool log_err(const char* message, ...);
	
	//Writes lots of stuff about platform to log file. There is no need to call this more than one time per run.
	void gl_log_params();
	
	//Prints shaders info log to console, but does NOT write it to log file
	void gl_print_shader_info_log(GLuint shader_index);
	
	//Checks if shader complied correctly. If not, returns false and writes notification to log file and calls _print_shader_info_log()
	bool gl_check_shader(GLuint shader);
	
	//Prints programs info log to console, but not to the log file
	void gl_print_program_info_log(GLuint program);

	//Checks if program is ok. If not, will write about it to log file and call _print_program_info_log()
	bool gl_check_program(GLuint program);

	bool check_opengl_error();

}

