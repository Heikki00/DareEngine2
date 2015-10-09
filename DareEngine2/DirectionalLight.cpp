#include "DirectionalLight.h"
#include "Shader.h"




DirectionalLight::DirectionalLight(const Vector3& color, const float intensity,
	int shandowMapSizeAsPowerOf2, float shandowArea, float shandowSoftness, float lightBleedReductionAmount, float minVariance) : BaseLight(color, intensity)
{
	float halfShandowArea = shandowArea / 2;

	BaseLight::setShader(Shader::makePtr("deferred/deferred-directional"));

	

	if (shandowMapSizeAsPowerOf2 != 0){
		BaseLight::setShandowInfo(ShandowInfo::makePtr(new ShandowInfo(Math::orthographicMatrix(-halfShandowArea, halfShandowArea, -halfShandowArea, halfShandowArea, -halfShandowArea, halfShandowArea),
			false, shandowMapSizeAsPowerOf2, shandowSoftness, lightBleedReductionAmount, minVariance)));
	}

	this->type = ComponentType::DIRECTIONALLIGHT;

}



Vector3 DirectionalLight::getDirection()
{
	return getTransform()->getWorldForward();

}