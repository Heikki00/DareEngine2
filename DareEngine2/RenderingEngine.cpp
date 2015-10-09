#include "RenderingEngine.h"
#include "GameObject.h"
#include "BaseLight.h"
#include "Shader.h"

#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshComponent.h"
#include "Time.h"

#include <exception>
#include <algorithm>
#include <functional>


#include "Uniform.h"
#include "UUCallback.h"




inline Shader* scast(void* val){ return (Shader*)val; }

Matrix4 RenderingEngine::biasMatrix = Math::scaleMatrix(0.5f) * Math::translationMatrix(Vector3(1, 1, 1));


RenderingEngine::RenderingEngine(){
	



	glDepthMask(GL_TRUE);
	

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_DEPTH_CLAMP);
	

	
	
	const int numGBuffers = 8;

	GLenum filters[numGBuffers] = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_LINEAR, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_LINEAR, GL_NEAREST };
	GLenum internalFormats[numGBuffers] = { GL_RGBA, GL_RGBA16F, GL_RGBA16F, GL_R32F, GL_RGBA, GL_RGBA16F, GL_RGBA16F, GL_R32F };
	GLenum formats[numGBuffers] = { GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA };
	GLenum attachments[numGBuffers] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	

	

	setTexture("gBuffer", Texture::makePtr(new Texture(4, 800, 600, GL_TEXTURE_2D, filters, internalFormats, formats, false, attachments)));
	setTexture("gBufferTransparency", Texture::makePtr(new Texture(4, 800, 600, GL_TEXTURE_2D, filters, internalFormats, formats, false, attachments)));
	
	



	setTexture("targetBuffer", Texture::makePtr(new Texture(0, 800, 600, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0)));
	setTexture("transparencyBuffer", Texture::makePtr(new Texture(0, 800, 600, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0)));
	
	setTexture("transparencyShandowColorBuffer", Texture::makePtr(new Texture(0, 1024, 1024, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0)));

	setTexture("TestTexture", Texture::makePtr(new Texture("res/textures/Grid.png")));

	


	int numShandowMaps = 10;

	

	
	for (int i = 0; i < numShandowMaps; i++){
		

		int shandowMapSize = 1 << (i + 1);

		shandowMaps.push_back(Texture::makePtr(new Texture(0, shandowMapSize, shandowMapSize, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0)));
		transparencyShandowMaps.push_back(Texture::makePtr(new Texture(0, shandowMapSize, shandowMapSize, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0)));
		shandowMapTempTargets.push_back(Texture::makePtr(new Texture(0, shandowMapSize, shandowMapSize, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0)));

	}
	setTexture("shandowMap", shandowMaps.at(0));
	


	canvas = new RenderCanvas;
	
	canvas->getMaterial()->setVector("color", Vector3(0, 0, 0));
	canvas->getMaterial()->setFloat("blue",1);


	altCamera = Camera::makePtr(70.f, (float)CoreEngine::window->getWidth() / (float)CoreEngine::window->getHeight(), 0.001f, 1000.0f);
	
	cameraObject = GameObject::makePtr();
	cameraObject->addComponent(altCamera);
	
		
	transparentOverride = false;

	

	setPointer("writeShader", new Shader("deferred/deferred-writer"));
	setPointer("defaultShader", new Shader("defaultShader"));
	setPointer("ambientShader", new Shader("deferred/deferred-ambient"));
	setPointer("shandowMapGenerator", new Shader("shandowMapGenerator"));
	setPointer("gausBlurFilter", new Shader("filter-gausBlur7x1"));
	setPointer("depthMapGenerator", new Shader("depthMapGenerator"));
	setPointer("transparentShader", new Shader("deferred/transparentBlend"));
	setPointer("multiAphaFilter", new Shader("filter-multiplyAlpha"));
	setPointer("testShader", new Shader("testShader"));

	
	Shader::addStaticCallback(UUCallback([](Transform* transform, Material* material, RenderingEngine* renderingEngine, const Shader& shader, Uniform& localUniform, Uniform& storedUniform){

		if (localUniform.getType() == "sampler2D"){


			int _pos = localUniform.getName().find_first_of('_', 3);
			std::string postfix = localUniform.getName().substr(_pos + 1, localUniform.getName().length() - _pos);

			bool isGbuffer = localUniform.getName().find("gBuffer") != localUniform.getName().npos && localUniform.getName().find("Transparency") == localUniform.getName().npos;


			if (isGbuffer && postfix.at(postfix.length() - 1) == 'T'){



				localUniform.setName("R_gBufferTransparency_" + postfix.substr(0, 1));


			}





			else if (renderingEngine->transparentOverride && isGbuffer){

				localUniform.setName("R_gBufferTransparency_" + postfix.substr(0, 1));

			}

		}


		return false;
		
	
	
	
	
	}
		
	
		));



	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	MappedValues::setVector("ambient", Vector3(0.1f, 0.1f, 0.1f));
	
	
	




	MappedValues::setFloat("useMVP", 1.0f);


	samplerMap["diffuse"] = 14;
	samplerMap["normalMap"] = 1;
	samplerMap["dispMap"] = 2;
	samplerMap["shandowMap"] = 3;
	samplerMap["gBuffer_0"] = 4;
	samplerMap["gBuffer_1"] = 5;
	samplerMap["gBuffer_2"] = 6;
	samplerMap["gBuffer_3"] = 7;
	samplerMap["gBufferTransparency_0"] = 8;
	samplerMap["gBufferTransparency_1"] = 9;
	samplerMap["gBufferTransparency_2"] = 10;
	samplerMap["gBufferTransparency_3"] = 11;
	samplerMap["transparencyShandowMap"] = 12;
	samplerMap["transparencyShandowColorBuffer"] = 13;

	CoreEngine::window->bindAsRenderTarget();


	
	

}



RenderingEngine::~RenderingEngine()
{
	

	delete canvas;

}

void RenderingEngine::render(GameObject* object){
	
	getTexture("gBuffer")->bindAsRenderTarget();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderAllAlpha(false, scast(getPointer("writeShader")), object);

	getTexture("gBufferTransparency")->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderAllAlpha(true, scast(getPointer("writeShader")), object);
	


	

	renderOpaque(object);
	renderTransparent(object);
	
	getTexture("targetBuffer")->bindAsRenderTarget();


	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);
	
	canvas->draw(this, getTexture("transparencyBuffer"), scast(getPointer("transparentShader")));

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	

	

	CoreEngine::window->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	

	canvas->draw(this, getTexture("targetBuffer"), scast(getPointer("defaultShader")), false);
	
	
	
	CoreEngine::window->swapBuffers();
	

}



void RenderingEngine::renderTransparent(GameObject* object){



	transparentOverride = true;
	
	


	getTexture("transparencyBuffer")->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	canvas->draw(this, nullptr, scast(getPointer("ambientShader")));


	for (auto& light : lights){

		activeLight = light;

		
		//generateShandowMaps(light, object);



		getTexture("transparencyBuffer")->bindAsRenderTarget();





		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_ALWAYS);


		canvas->draw(this, nullptr, light->getShader());
		



		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}




	
	transparentOverride = false;





}


void RenderingEngine::renderOpaque(GameObject* object){

	

	
	getTexture("targetBuffer")->bindAsRenderTarget();


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	canvas->draw(this, nullptr, scast(getPointer("ambientShader")));

	for (auto& light : lights){

		activeLight = light;


		//generateShandowMaps(light, object);
		
		
		getTexture("targetBuffer")->bindAsRenderTarget();





		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_ALWAYS);


		
		canvas->draw(this, nullptr, light->getShader());

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);








	}


}

void RenderingEngine::generateShandowMaps(std::shared_ptr<BaseLight> light, GameObject* root){


	auto shandowInfo = light->getShandowInfo();


	int shandowMapIndex = 0;

	lightMatrix.identity();

	if (shandowInfo) shandowMapIndex = shandowInfo->getShandowMapSizeAsPowerOf2() - 1;

	setTexture("transparencyShandowMap", transparencyShandowMaps.at(shandowMapIndex));
	transparencyShandowMaps.at(shandowMapIndex)->bindAsRenderTarget();
	glClearColor(1, 1, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	setTexture("shandowMap", shandowMaps.at(shandowMapIndex));
	shandowMaps.at(shandowMapIndex)->bindAsRenderTarget();
	glClearColor(1, 1, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	getTexture("transparencyShandowColorBuffer")->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	if (shandowInfo){

		altCamera->setProjection(shandowInfo->getProjection());
		altCamera->getTransform()->setPosition(light->getTransform()->getTransformedPos());
		altCamera->getTransform()->setRotation(light->getTransform()->getTransformedRot());


		lightMatrix = RenderingEngine::biasMatrix * altCamera->getViewProjection();

		setFloat("shandowVarianceMin", shandowInfo->getMinVariance());
		setFloat("shandowLightBleedReduction", shandowInfo->getLightBleedReduction());
		bool flip = shandowInfo->getFlipfaces();


		


		Camera* temp = mainCamera;
		mainCamera = altCamera.get();
		glEnable(GL_CULL_FACE);

		if (flip) glCullFace(GL_FRONT);

		shandowMaps.at(shandowMapIndex)->bindAsRenderTarget();

		renderAllAlpha(false, scast(getPointer("depthMapGenerator")), root);



		transparencyShandowMaps.at(shandowMapIndex)->bindAsRenderTarget();

		renderAllAlpha(true, scast(getPointer("depthMapGenerator")), root);

		getTexture("transparencyShandowColorBuffer")->bindAsRenderTarget();

		renderAllAlpha(true, scast(getPointer("defaultShader")), root);



		if (flip) glCullFace(GL_BACK);

		glDisable(GL_CULL_FACE);
		mainCamera = temp;

		if (shandowInfo->getShandowSoftness() != 0){
		//	blurShandowMap(shandowMapIndex, shandowInfo->getShandowSoftness(), pcast(getPointer("gausBlurFilter")));
		}

	}
	else{
		setFloat("shandowVarianceMin", 0.002f);
		setFloat("shandowLightBleedReduction", 0.2f);


	}






}



void RenderingEngine::applyFilter(Shader* filter, std::shared_ptr<Texture> source, std::shared_ptr<Texture> target){

	if (source == target){
		DebugUtility::log_err("ERROR: Tried to filter texture to itself\n");
		return;
	}

	if (!target)
		CoreEngine::window->bindAsRenderTarget();

	else
		target->bindAsRenderTarget();

	setTexture("filterTexture", source);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	

	
	
	canvas->draw(this, nullptr, filter);
	
	

	setTexture("filterTexture", nullptr);
}


void RenderingEngine::blurShandowMap(int shandowMapIndex, float blurAmount, Shader* blurFilter){
	setVector("blurScale", Vector3(blurAmount / (shandowMaps.at(shandowMapIndex)->getWidth()), 0.f, 0.f));
	applyFilter(blurFilter, shandowMaps.at(shandowMapIndex), shandowMapTempTargets.at(shandowMapIndex));

	setVector("blurScale", Vector3(0.0f, blurAmount / (shandowMaps.at(shandowMapIndex)->getWidth()), 0.f));
	applyFilter(blurFilter, shandowMapTempTargets.at(shandowMapIndex), shandowMaps.at(shandowMapIndex));

	setVector("blurScale", Vector3(blurAmount / (transparencyShandowMaps.at(shandowMapIndex)->getWidth()), 0.f, 0.f));
	applyFilter(blurFilter, transparencyShandowMaps.at(shandowMapIndex), shandowMapTempTargets.at(shandowMapIndex));

	setVector("blurScale", Vector3(0.0f, blurAmount / (transparencyShandowMaps.at(shandowMapIndex)->getWidth()), 0.f));
	applyFilter(blurFilter, shandowMapTempTargets.at(shandowMapIndex), transparencyShandowMaps.at(shandowMapIndex));




}





void RenderingEngine::renderAllAlpha(bool transparent, Shader* shader, GameObject* root){

	auto objList = root->getAllAttached();

	

	for (auto& o : objList){


		if (!transparent){
			//if current object is not in transparentObjects
			
			if (std::find(transparentObjects.begin(), transparentObjects.end(), o.get()) == transparentObjects.end()){
				
				
				
				o->render(shader, this);

			}
		}
		else{
			//if current object is in transparentObjects
			if (std::find(transparentObjects.begin(), transparentObjects.end(), o.get()) != transparentObjects.end()){
				




				o->render(shader, this);

			}





		}
	
	
	
	
	
	
	}



}








#pragma region RenderCanvas


RenderCanvas::RenderCanvas(){
	
	std::vector<Vertex> vert = {
		Vertex(Vector3(-1, 1, 0),  Vector3(0, 1, 0)),
		Vertex(Vector3(-1, -1, 0), Vector3(0, 0, 0)),
		Vertex(Vector3(1, -1, 0),  Vector3(1, 0, 0)),
		Vertex(Vector3(1, 1, 0),   Vector3(1, 1, 0))
	};
	
	std::vector<unsigned int> ind = {0,1,2,2,3,0};

	object = new GameObject;
	
	material = Material::makePtr();

	object->addComponent(MeshComponent::makePtr(new MeshComponent(Mesh::makePtr(new Mesh(vert, ind)), material)));

	shader = new Shader("defaultShader");


}


RenderCanvas::~RenderCanvas(){

	
}

void RenderCanvas::draw(RenderingEngine* renderingEngine, std::shared_ptr<Texture> texture, Shader* shader, bool useMVP){
	float temp = renderingEngine->getFloat("useMVP");
	renderingEngine->setFloat("useMVP", useMVP ? 1.0f : 0.0f);
	
	if(texture) material->setTexture("diffuse", texture);
	
	
	if (shader == nullptr){
		shader = this->shader;
	}
	
	object->render(shader, renderingEngine);

	renderingEngine->setFloat("useMVP", temp);
	
}




#pragma endregion

