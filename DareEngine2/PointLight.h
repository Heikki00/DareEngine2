
#include <memory>
#include "BaseLight.h"
#include "DareMath.h"

#ifdef BASELIGHTH

#ifndef POINTLIGHTH
#define POINTLIGHTH






class PointLight : public BaseLight{
public:
	PointLight(const Vector3& color, const float intensity, const Vector3& attenuation);

	~PointLight(){}

	

	

	inline float getRange() const { return range; }
	inline void setRange(float val) { range = val; }

	inline float getConstant() const{ return this->attenuation.x; }
	inline void setConstant(const float val) { this->attenuation.x = val; }

	inline float getLinear() const{ return this->attenuation.y; }
	inline void setLinear(const float val) { this->attenuation.y = val; }

	inline float getExponent() const{ return this->attenuation.z; }
	inline void setExponent(const float val) { this->attenuation.z = val; }

	inline const Vector3& getAttenuation() const{ return this->attenuation; }
	inline void setAttenuation(const Vector3& val){ this->attenuation = val; }

	inline static std::shared_ptr<PointLight> makePtr(PointLight* val){ return  std::shared_ptr<PointLight>(val); }
	

	


private:


	float range;

	Vector3 attenuation;


};



#endif
#endif























