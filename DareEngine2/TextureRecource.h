#pragma once
#include <glew.h>
#include <memory>
#include "Utility.h"
#include <string>
#include <vector>

class TextureResource{
public:


	TextureResource(GLenum textureTarget, int numTextures, unsigned char** data, int width, int height, GLenum* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments);

	~TextureResource();

	
	void bind(GLint texture);
	void bindAsRenderTarget();

	bool isTexture();


	inline static std::shared_ptr<TextureResource> makePtr(TextureResource* val){ return std::shared_ptr<TextureResource>(val); }
	
	inline int getWidth() const{ return width; }
	inline int getHeight() const{ return height; }

	inline int getNumTextures() const{ return numTextures; }

	
	inline GLuint getID(int index) const{ return ids[index]; }

	inline GLenum getAttachment(int index) const{ return attachments[index]; }


protected:
	inline void setID(int index, GLuint ID){ ids[index] = ID; }
	inline void setAttachment(int index, GLenum attachment){ attachments[index] = attachment; }

	void initTextures(unsigned char** data, GLenum* filters, GLenum* internalFormat, GLenum* format, bool clamp);
	void initRenderTargets(GLenum* attachments);

	friend class Texture;


private:
	int numTextures;
	GLuint* ids;
	GLuint frameBuffer;
	GLuint renderBuffer;
	
	GLenum* attachments;


	GLenum textureTarget;
	int width, height;
};







