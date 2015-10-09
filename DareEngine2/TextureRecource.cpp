#include "TextureRecource.h"
#include "DebugUtility.h"
#include <stdio.h>

TextureResource::TextureResource(GLenum textureTarget, int numTextures, unsigned char** data, int width, int height, GLenum* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments){
	this->ids = new GLuint[numTextures];

	glGenTextures(numTextures, ids);
	
	
	this->width = width;
	this->height = height;
	this->textureTarget = textureTarget;
	this->numTextures = numTextures;
	frameBuffer = 0;
	renderBuffer = 0;

	initTextures(data, filters, internalFormat, format, clamp);
	
	this->attachments = attachments;

	initRenderTargets(attachments);

	

}




TextureResource::~TextureResource(){
	
	if(glIsTexture(ids[0])) glDeleteTextures(numTextures, ids);
	if (glIsFramebuffer(frameBuffer)) glDeleteFramebuffers(1, &frameBuffer);
	if (glIsRenderbuffer(renderBuffer)) glDeleteRenderbuffers(1, &renderBuffer);
	if (ids) delete[] ids;
}


void TextureResource::initTextures(unsigned char** data, GLenum* filters, GLenum* internalFormat, GLenum* format, bool clamp){
	
	
	for (int i = 0; i < numTextures; i++){

		
		glBindTexture(textureTarget, ids[i]);

		/*glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);*/

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		
		
		if(filters[i] != GL_NEAREST)glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		if (clamp){
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);


		}


		glTexImage2D(textureTarget, 0, internalFormat[i],
			width, height,
			0, format[i], GL_UNSIGNED_BYTE, data[i]);
		
	

		if (filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR){
			
			glGenerateMipmap(textureTarget);
		
			GLfloat maxAntisotropy;
			
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAntisotropy);
			

			glTexParameteri(textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAntisotropy);
		}
		else{
			glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}



	}
	
	

	glBindTexture(textureTarget, 0);

}


void TextureResource::initRenderTargets(GLenum* attachments){
	if (!attachments){
		return;
	}

	GLenum drawbuffers[32];

	bool hasDepht = false;
	int drawableBuffers = 0;

	for (int i = 0; i < numTextures; i++){
	
		if (attachments[i] == GL_DEPTH_ATTACHMENT){
			hasDepht = true;
			drawbuffers[i] = GL_NONE;
		}
		else{
			drawbuffers[i] = attachments[i];
			++drawableBuffers;
		}

		if (attachments[i] == GL_NONE){
			continue;
		}
		
		if (!frameBuffer){
			glGenFramebuffers(1, &frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		}
		

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], textureTarget, ids[i], 0);
	
	}
	if (frameBuffer == 0){
		return;
	}
	
	
	if (!hasDepht){

		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	}
	
	
	glDrawBuffers(drawableBuffers, drawbuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		DebugUtility::log_err("ERROR: Framebuffer creation failed(in textureResource)\n");
		//std::cout << std::hex  <<glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}




bool TextureResource::isTexture(){
	for (int i = 0; i < numTextures; i++){
		if (!glIsTexture(ids[i])){
			return false;
		}
	}
	return true;

}

void TextureResource::bind(GLint texture){
	
	
	glBindTexture(textureTarget, ids[texture]);
	

}


void TextureResource::bindAsRenderTarget(){
	glBindTexture(GL_TEXTURE_2D, 0);
	//std::cout << (bool)glIsFramebuffer(frameBuffer) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);

}
