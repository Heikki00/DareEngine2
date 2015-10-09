#include "TestGame.h"
#include "CoreEngine.h"
#include "FreeLook.h"

std::shared_ptr<GameObject> sunObject = GameObject::makePtr();
auto teaObject = GameObject::makePtr();

TestGame::TestGame(){
	Input::mouseTrap(true);
}



TestGame::~TestGame(){


	
	
}



void TestGame::init()
{
	



	auto mesh1 = Mesh::makePtr(new Mesh("res/objects/plane.obj"));


	auto material = Material::makePtr();
	material->setTexture("diffuse", Texture::makePtr(new Texture("res/textures/window.jpg")));
	material->setTexture("normalMap", Texture::makePtr(new Texture("res/textures/Window_NORM.jpg")));
	material->setTexture("dispMap", Texture::makePtr(new Texture("res/textures/Window_DISP.jpg")));
	material->setFloat("opacity", 0.7f);

	material->setFloat("specularIntensity",1.0f);
	material->setFloat("specularPower", 400.0f);


	auto material2 = Material::makePtr();
	material2->setTexture("diffuse", Texture::makePtr(new Texture("res/textures/bricks2.jpg")));	
	material2->setTexture("normalMap", Texture::makePtr(new Texture("res/textures/bricks2_normal.jpg")));	
	material2->addDisplacementValues(0.02f, -1.0f, Texture::makePtr(new Texture("res/textures/bricks2_disp.jpg")));
	
	material2->setFloat("specularIntensity", 1.f);
	material2->setFloat("specularPower",510.0f);


	
	auto floorObject = GameObject::makePtr();
	floorObject->addComponent(MeshComponent::makePtr(new MeshComponent(mesh1, material2)));
	
	floorObject->getTransform()->translate(Vector3(0, -1, 0) * 6);
	floorObject->getTransform()->rotate(Quaternion(Vector3(1, 0, 0), PI / 2.0));
	floorObject->getTransform()->setScale(10);


	auto planeObject = GameObject::makePtr();
	planeObject->addComponent(MeshComponent::makePtr(new MeshComponent(Mesh::makePtr(new Mesh("res/objects/plane.obj")), material)));
	
	planeObject->getTransform()->setScale(10.f);
	planeObject->getTransform()->rotate(Vector3(1, 0, 0), Math::rad(90.f));
	planeObject->getTransform()->translate(Vector3(0, 0, 1) * 6);



	auto cameraObject = GameObject::makePtr();
	auto camera = Camera::makePtr(Math::rad(70.0f), 800.f / 600.f, 0.1f, 500.f);
	

	cameraObject->addComponent(camera);
	cameraObject->addComponent(FreeLook::makePtr());

	
	teaObject->addComponent(MeshComponent::makePtr(new MeshComponent(Mesh::makePtr(new Mesh("res/objects/sphere.obj")), material2)));
	

	teaObject->getTransform()->setScale(1);
	//teaObject->getTransform()->setScale(10.f, 10.f, 10.f);
	//teaObject->getTransform()->translate(Vector3(0, 0, 1), 0);
	//teaObject->getTransform()->offset(Vector3(0, 1, 0), 5);
	teaObject->name = "teaObject";
	//teaObject->getTransform()->rotate(Vector3(1, 0, 0), 45.f);



	sunObject->addComponent(DirectionalLight::makePtr(new DirectionalLight(Vector3(1,1,1), 0.2f, 10, 80.0f, 1.0f, 0.4f, 0.0002f)));
	
	sunObject->getTransform()->rotate(Vector3(1, 0, 0), -90.f);
	
	


	auto cubeObject = GameObject::makePtr();
	cubeObject->addComponent(MeshComponent::makePtr(new MeshComponent(Mesh::makePtr(new Mesh("res/objects/cube.obj")), material)));
	cubeObject->getTransform()->translate(Vector3(1, 0, 1) * 5.f);
	cubeObject->getTransform()->translate(Vector3(0, 1, 0) * -5.5f);
	cubeObject->getTransform()->setScale(0.5f);


	getRootObject()->addChild(sunObject);
	//getRootObject()->addChild(teaObject);
	getRootObject()->addChild(teaObject);

	getRootObject()->addChild(cameraObject);
	getRootObject()->addChild(floorObject);
	
	//getRootObject()->addChild(planeObject);
	//getRootObject()->addChild(cubeObject);
	
	
}

void TestGame::update(float delta)
{
	
	
	
	

	


	Game::update(delta);
	


}





void TestGame::input(float delta){
	Game::input(delta);




}
