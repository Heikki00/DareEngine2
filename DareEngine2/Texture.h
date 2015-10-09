#pragma once
#include <glew.h>
#include "DebugUtility.h"
#include "Utility.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <IL/il.h>
#include <IL/ilu.h>
#include <vector>

class TextureResource;

#define LOADED_FROM_DATA "LOADED_FROM_DATA"


class Texture
{
public:
	
	Texture(const std::string& filename, GLenum textureTarget = GL_TEXTURE_2D, GLenum filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attacment = GL_NONE);
	Texture(unsigned char* data, int width, int height, GLenum textureTarget = GL_TEXTURE_2D, GLenum filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attacment = GL_NONE);
	
	Texture(int numTextures, int width, int height, GLenum textureTarget, GLenum* filters, GLenum* internalFormats, GLenum* formats, bool clamp, GLenum* attachments);

	~Texture();

	//Tells OpenGL to use this texture. textureUnit must be between 1 and 32, otherwise writes to error file
	void bind(GLuint textureUnit, int texture = 0);
	
	

	void bindAsRenderTarget();




	int getTextureAmount() const;


	inline static std::shared_ptr<Texture> makePtr(Texture* texture){ return std::shared_ptr<Texture>(texture);  }
	
	int getWidth() const;
	int getHeight() const;


private:

	

	ILuint loadTexture(const std::string& filename);


	std::shared_ptr<TextureResource> resource;
	static std::unordered_map<std::string, std::shared_ptr<TextureResource>>* loadedTextures;

	std::string filename;

};

