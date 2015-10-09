

#include <memory>
#include "BaseLight.h"
#include "DareMath.h"
#ifdef BASELIGHTH

#ifndef DIRECTIONALLIGHTH
#define DIRECTIONALLIGHTH

class RenderingEngine;
class DirectionalLight : public BaseLight{
public:
	DirectionalLight(const Vector3& color, const float intensity, int shandowMapSizeAsPowerOf2 = 10, float shandowArea = 40.f, float shandowSoftness = 1.0f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f);
	~DirectionalLight(){}
	

	Vector3 getDirection();
	

	inline static std::shared_ptr<DirectionalLight> makePtr(DirectionalLight* val){ return std::shared_ptr<DirectionalLight>(val); }

private:

	

};



#endif
#endif








