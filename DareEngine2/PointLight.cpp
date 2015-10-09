#include "PointLight.h"
#include "Shader.h"



PointLight::PointLight(const Vector3& color, const float intensity, const Vector3& attenuation) :
BaseLight(color, intensity)
{
	this->attenuation = attenuation;
	
	float max = (color.x > color.y ? color.x : color.y) < color.z ? color.z : (color.x > color.y ? color.x : color.y);

	float a = attenuation.z;
	float b = attenuation.y;
	float c= attenuation.x - 256 * intensity * max;

	this->range = (float)(-b, sqrtf(b * b - 4 * a * c)) / (2 * a);
	
	if (range != range){
		range = 500.f;
	}

	BaseLight::setShader(Shader::makePtr("deferred/deferred-point"));

	this->type = ComponentType::POINTLIGHT;
}




