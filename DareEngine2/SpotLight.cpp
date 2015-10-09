#include "SpotLight.h"
#include "Shader.h"


SpotLight::SpotLight(const Vector3& color, const float intensity, const Vector3& attenuation, int shandowMapPowerOf2,
	float viewAngle, bool flipfaces, float shandowSoftness, float lightBleedReduction, float minVariance) : PointLight(color, intensity, attenuation)
{
	this->cutoff = cos(viewAngle / 2);



	if (shandowMapPowerOf2 != 0){
		setShandowInfo(ShandowInfo::makePtr(new ShandowInfo(Math::perspectiveMatrix(viewAngle, 1.f, 0.1f, this->getRange()), flipfaces, shandowMapPowerOf2, shandowSoftness, lightBleedReduction, minVariance)));
	}



	PointLight::setShader(Shader::makePtr("deferred/deferred-spot"));

	this->type = ComponentType::SPOTLIGHT;

}



Vector3 SpotLight::getDirection()
{

	return getTransform()->getTForward();
}
