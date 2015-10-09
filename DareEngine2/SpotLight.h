#include "DareMath.h"
#include <memory>
#include "PointLight.h"


#ifdef POINTLIGHTH

#ifndef SPOTLIGHTH
#define SPOTLIGHTH

class SpotLight : public PointLight{
public:
	SpotLight(const Vector3& color, const float intensity, const Vector3& attenuation, int shandowMapPowerOf2 = 0,
		float viewAngle = Math::rad(170.f), bool flipfaces = false, float shandowSoftness = 1.0f, float lightBleedReduction = 0.2f, float minVariance = 0.00002f);
	
	~SpotLight(){}


	Vector3 getDirection();

	inline float getCutoff() const { return cutoff; }
	inline void setCutoff(const float val) { cutoff = val; }

	
	inline static std::shared_ptr<SpotLight> makePtr(SpotLight* val){ return std::shared_ptr<SpotLight>(val); }


private:
	
	
	float cutoff;

};


#endif
#endif
