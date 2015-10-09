#pragma once
#include <vector>
#include <memory>
#include "GameComponent.h"
#include "Transform.h"
#include "Input.h"

class RenderingEngine;
class Shader;
class CoreEngine;

//Node of the scene graph. Contain children(witch can have their own children) and components. Components will 
//define the behavior of the game object, so create then, DO NOT USE THIS AS AN INTERFACE. 
class GameObject : public std::enable_shared_from_this<GameObject>{
public:
	GameObject();
	~GameObject();

	//Adds child to this node. everything done to this node, will be done to all children
	void addChild(std::shared_ptr<GameObject> child);

	//Adds component to this node. Components define behavior of the node. When node's update() is called, components update() is called etc.
	void addComponent(std::shared_ptr<GameComponent> component);

	//Processes input for this GameObject
	void input(float delta);

	//Calls render() for all components of this GameObject. Shader is shader to be used in rendering.
	void render(Shader* shader, RenderingEngine* renderingEngine);

	//Calls update() for all components of this GameObject.
	void update(float delta);


	//Calls input() for all components and children (Incluing this)
	void inputAll(float delta);

	//Calls render() for all components and children. Shader is shader to be used in rendering.
	void renderAll(Shader* shader, RenderingEngine* renderingEngine);

	//Calls update() for all components and children
	void updateAll(float delta);

	//Returns vector of all GameObjects that are attatced to this GameObject (Including children's children etc.). Starts with this GameObject
	std::vector<std::shared_ptr<GameObject>> getAllAttached();

	//Returs a vector of GameObjects that are childs of this GameObject. Does not include this GameObject
	std::vector<std::shared_ptr<GameObject>> getAllChildren();

	//Returns Child that corresponds to index. Returns nullptr and logs error if index is out if borders
	std::shared_ptr<GameObject> getChild(int index);

	//Returns number of children this GameObject has
	inline int getChildCount() const{ return children.size(); }

	//Removes child and all its children
	void removeChild(int index);

	void removeAllChildren();

	std::shared_ptr<GameObject> findByName(const std::string& name);

	//Retruns a vector of this objects componenst
	std::vector<std::shared_ptr<GameComponent>> getComponents();

	std::shared_ptr<GameComponent> getComponent(int index);

	int getComponentCount() const{ return components.size(); }


	void removeComponent(int index);

	void removeAllComponents();


	std::vector<std::shared_ptr<GameComponent>> getComponetsOfType(ComponentType type);

	inline bool containsType(ComponentType type){ return getComponetsOfType(type).empty() ? false : true; }

	//Gives this GameObject and all it's components and children reference to CoreEngine. Used to get stuff from RenderingEngine etc.
	void setEngine(CoreEngine* engine);
	


	//Returns pointer to Transform object of this GameObject. Pointer can be freely modified;
	inline Transform* getTransform(){ return transform; }

	//Returns std::shared_ptr to new GameObject
	inline static std::shared_ptr<GameObject> makePtr(){ return std::make_shared <GameObject>(); }
	
	std::string name;
protected:

	


private:
	std::vector<std::shared_ptr<GameObject>> children;
	std::vector<std::shared_ptr<GameComponent>> components;
	Transform* transform;
	CoreEngine* engine;
};







