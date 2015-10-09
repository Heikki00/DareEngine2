#include "MeshComponent.h"
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "CoreEngine.h"

MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material){
	this->mesh = mesh;
	this->material = material;
	this->type = ComponentType::MESH;
}


MeshComponent::~MeshComponent(){
	

}

void MeshComponent::render(Shader* shader, RenderingEngine* renderingEngine){
	if (!_isVisible) return;
	
	shader->bind();
	
	shader->updateUniforms(getParent()->getTransform(), material.get(), renderingEngine);
	

	mesh->draw();
}


void MeshComponent::addToEngine(CoreEngine* engine){

	this->engine = engine;

	if (material->getFloat("opacity") < 1.0f){
		engine->getRenderingEngine()->addTransparentObject(this->getParent());
	
	}


}