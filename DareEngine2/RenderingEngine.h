#pragma once



#include "CoreEngine.h"
#include <memory>
#include "DareMath.h"
#include "Material.h"
#include <glew.h>
#include <unordered_map>
#include "MappedValues.h"
#include "Camera.h"
#include "Transform.h"


class BaseLight;
class Camera;
class GameObject;
class RenderCanvas;
class Shader;
class Mesh;
class Uniform;

class RenderingEngine : public MappedValues
{
public:
	RenderingEngine();
	~RenderingEngine();

	void render(GameObject* object);
	
	 
	inline Camera* getMainCamera(){ return this->mainCamera; }
	

	inline std::shared_ptr<BaseLight> getActiveLight(){ return activeLight; }
	


	inline void addLight(std::shared_ptr<BaseLight> val){ lights.push_back(val); }

	inline void addCamera(Camera* camera){	mainCamera = camera; }
	
	inline void addTransparentObject(GameObject* object){ transparentObjects.push_back(object); }

	inline GLuint getSamplerSlot(const std::string& samplerName){ return samplerMap[samplerName]; }

	


	inline Matrix4 getLightMatrix() const { return lightMatrix; }

protected:

	inline void setMainCamera(Camera* cam){ this->mainCamera = cam; }
	bool transparentOverride;

	friend class RenderCanvas;
	friend class Shader;
	friend bool updateUniformCallback(Transform* transform, Material* material, RenderingEngine* renderinEngine, const Shader& shader, Uniform* uniform);

private:
	
	
	void renderTransparent(GameObject* object);
	void renderOpaque(GameObject* object);
	void generateShandowMaps(std::shared_ptr<BaseLight> light, GameObject* root);

	int numShandowMaps;
	std::vector<std::shared_ptr<Texture>> shandowMaps;
	std::vector<std::shared_ptr<Texture>> transparencyShandowMaps;
	std::vector<std::shared_ptr<Texture>> shandowMapTempTargets;

	std::vector<std::shared_ptr<BaseLight>> lights;
	std::shared_ptr<BaseLight> activeLight;
	Matrix4 lightMatrix;


	void applyFilter(Shader* filter, std::shared_ptr<Texture> source, std::shared_ptr<Texture> target);
	void blurShandowMap(int shandowMapIndex, float blurAmount, Shader* blurFilter);

	Camera* mainCamera;
	
	std::shared_ptr<Camera> altCamera;
	std::shared_ptr<GameObject> cameraObject;
	
	std::vector<GameObject*> transparentObjects;

	RenderCanvas* canvas;
	
	

	std::unordered_map<std::string, GLuint> samplerMap;
	
	void renderAllAlpha(bool transparency, Shader* shader, GameObject* root);
	

	static Matrix4 biasMatrix;

};



class RenderCanvas{
public:

	RenderCanvas();

	~RenderCanvas();

	void draw(RenderingEngine* renderingEngine, std::shared_ptr<Texture> texture, Shader* shader = nullptr, bool useMVP = false);

	std::shared_ptr<Material> getMaterial(){ return material; }

private:
	GameObject* object;
	
	Shader* shader;
	std::shared_ptr<Material> material;



};

