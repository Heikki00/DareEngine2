#pragma once
#include <memory>
#include "GameComponent.h"



class RenderingEngine;
class Shader;
class Material;
class Mesh;

class MeshComponent : public GameComponent
{
public:
	MeshComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	~MeshComponent();

	void render(Shader* shader, RenderingEngine* renderingEngine) override;

	void addToEngine(CoreEngine* engine) override;

	inline static std::shared_ptr<MeshComponent>makePtr(MeshComponent* val){ return std::shared_ptr<MeshComponent>(val); }


	inline std::shared_ptr<Mesh> getMesh(){ return mesh; }
	inline void setMesh(std::shared_ptr<Mesh> mesh){ this->mesh = mesh; }

	inline std::shared_ptr<Material> getMaterial(){ return material; }
	inline void setMaterial(std::shared_ptr<Material> material){ this->material = material; }

	void setVisible(bool val){ _isVisible = val; }
	bool isVisible() const{ return _isVisible; }

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	CoreEngine* engine;
	bool _isVisible;

	



};

