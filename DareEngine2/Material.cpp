#include "Material.h"
#include "glew.h"


Material::Material()
{
	
	setTexture("diffuse", Texture::makePtr(new Texture("res/textures/Grid.png")));
	setTexture("normalMap", Texture::makePtr(new Texture("res/textures/default_normal.jpg")));
	

	setTexture("dispMap", Texture::makePtr(new Texture("res/textures/default_disp.png")));
	setFloat("dispMapScale", 0.04f);
	setFloat("dispMapBias", 0.0f);
	setFloat("opacity", 1.0f);
	
}



Material::~Material(){
	
}





void Material::addDisplacementValues(float scale, float offset, std::shared_ptr<Texture> texture){
	if (texture){
		setTexture("dispMap", texture);
	}

	
	float baseBias = scale / 2.0f;

	setFloat("dispMapScale", scale);
	setFloat("dispMapBias", -baseBias + baseBias * offset);
	




}