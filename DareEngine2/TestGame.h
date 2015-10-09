#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "Shader.h"
#include "Transform.h"
#include "Input.h"

#include "Camera.h"
#include "Material.h"

#include "BaseLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <memory>
#include "Game.h"

#include "GameObject.h"
#include "MeshComponent.h"


class TestGame : public Game
{
public:
	TestGame();
	~TestGame();

	//Handles games input
//	void input();

	//Updates the game
	void update(float delta) override;

	//Renders the game
	//void render();

	void init() override;

	void input(float delta) override;


private:
	
};



