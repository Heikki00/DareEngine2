#pragma once
#include "GameComponent.h"
#include "DareMath.h"
#include <memory>
#include "CoreEngine.h"



class FreeLook :
	public GameComponent
{
public:
	FreeLook();

	void input(float delta) override;

	inline void addToEngine(CoreEngine* engine) override{ this->engine = engine; }


	~FreeLook();

	inline static std::shared_ptr<FreeLook> makePtr(){ return std::make_shared<FreeLook>(); }

private:

	void doRotation(const Vector3& axis, float angle);

	CoreEngine* engine;

};

