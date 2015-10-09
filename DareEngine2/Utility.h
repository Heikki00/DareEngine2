#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "DebugUtility.h"
#include <sstream>
#include <stdio.h>

#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <memory>

class Texture;

namespace Utilitys{

	
	//Standard function for reading text from files. If file does not exist, sends error message to log_err and returns empty string
	std::string readFile(const std::string& filename);

	//Reads file to stringstream and returns it
	std::stringstream readFileToStream(const std::string& filename);

	//Splits src by demitter and puts substrings into vector
	std::vector<std::string> splitString(const std::string& src, char demitter);

	std::string enumToType(GLenum typeEnum);

	//returns size of all basic uniform types
	unsigned int uniformTypeToSize(const std::string& type);

	//Simple template class that contains two values
	template<typename T, typename Y = T>
	struct Pair{
		Pair(T val1, Y val2){
			this->val1 = val1;
			this->val2 = val2;
		}

		T val1;
		Y val2;

		bool operator ==(const Pair& other){
			return (val1 == other.val1 && val2 == other.val2);
		}


	};

	
	 

	

}