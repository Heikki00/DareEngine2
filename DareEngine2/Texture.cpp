#include "Texture.h"
#include <IL\ilut.h>
#include "TextureRecource.h"
#include <exception>


std::unordered_map<std::string, std::shared_ptr<TextureResource>>* Texture::loadedTextures = new std::unordered_map<std::string, std::shared_ptr<TextureResource>>;

Texture::Texture(unsigned char* data, int width, int height, GLenum textureTarget, GLenum filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attacment){
	this->filename = LOADED_FROM_DATA;
	

	
	
	this->resource = TextureResource::makePtr(new TextureResource(textureTarget, 1, &data, width, height, &filter, &internalFormat, &format , clamp, &attacment));


	if (resource->isTexture())
		DebugUtility::log("Successfully loaded and created texture %s\n", filename.c_str());

}

Texture::Texture(const std::string& filename, GLenum textureTarget, GLenum filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attacment){


	this->filename = filename;

	

	if (loadedTextures->find(filename) != loadedTextures->end()){
		resource = (*loadedTextures)[filename];

	}
	else{
	
	
			
	
			


		auto ilID = loadTexture(filename);

		ilBindImage(ilID);
		unsigned char* temp = ilGetData();

		
		this->resource = TextureResource::makePtr(new TextureResource(textureTarget, 1, &temp, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), &filter, &internalFormat, &format, clamp, &attacment));

		ilDeleteImage(ilID);
		
		if (resource->isTexture())
			DebugUtility::log("Successfully loaded and created texture %s\n", filename.c_str());

		

		
	(*loadedTextures)[filename] = resource;
		
	}

	




}


Texture::Texture(int numTextures, int width, int height, GLenum textureTarget, GLenum* filter, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments){
	this->filename = LOADED_FROM_DATA;

	unsigned char** data = new unsigned char*[numTextures];

	std::fill_n(data, numTextures, nullptr);

	this->resource = TextureResource::makePtr(new TextureResource(textureTarget, numTextures, data, width, height, filter, internalFormat, format, clamp, attachments));

	if (resource->isTexture())
		DebugUtility::log("Successfully loaded and created texture %s\n", filename.c_str());



}





Texture::~Texture(){
	if (filename == LOADED_FROM_DATA || !loadedTextures){
		return;
	}

	if (resource.use_count() == 2){
		
		loadedTextures->erase(filename);

	}



}

void Texture::bind(GLuint position, int texture){
	if (position < 0 || position > 31){
		DebugUtility::log_err("ERROR: Requested to bind texture %1 to invalid texture uint: %s\n",filename, position);
		return;
	}
	if (!resource->isTexture()){
		DebugUtility::log_err("ERROR: Requested to bind invalid texture %s\n", filename);
		return;
	}
	

	glActiveTexture(GL_TEXTURE0 + position);
	resource->bind(texture);



}

void Texture::bindAsRenderTarget(){
	resource->bindAsRenderTarget();
}

ILuint Texture::loadTexture(const std::string& filename){


	ilInit();
	iluInit();

	ILuint ilID;


	ilGenImages(1, &ilID);

	ilBindImage(ilID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if (!ilLoad(IL_TYPE_UNKNOWN, (ILstring)filename.c_str())){
		DebugUtility::log_err("ERROR: Failed to load image %s\n", filename.c_str());
		return ilID;
	}
	
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	return ilID;




}


int Texture::getWidth() const{ return resource->getWidth(); }
int Texture::getHeight() const{ return resource->getHeight(); }

int Texture::getTextureAmount() const{ return resource->getNumTextures(); }