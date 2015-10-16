
#pragma once
#include "FestusMath.h"
#include <memory>
#include "GameComponent.h"
#include "CoreEngine.h"



class Shader;


class ShandowInfo{
public:
	ShandowInfo(const Matrix4& projection, bool flipfaces = false, int shandowMapSizeAsPowerOf2 = 0,  float shandowSoftness = 1.0f, float lightBleedReduction = 0.2f, float minVariance = 0.00002f){
		this->projection = projection;
		
		this->shandowMapSizeAsPowerOf2 = shandowMapSizeAsPowerOf2;
		this->flipFaces = flipFaces;
		this->shandowSoftness = shandowSoftness;
		this->lightBleedReduction = lightBleedReduction;
		this->minVariance = minVariance;
	}

	inline Matrix4 getProjection() const{ return projection; }
	inline void setProjection(const Matrix4& projection){ this->projection = projection; }

	
	inline bool getFlipfaces() const{ return flipFaces; }
	inline float getShandowSoftness() const{ return shandowSoftness; }
	inline float getLightBleedReduction() const{ return lightBleedReduction; }
	inline float getMinVariance() const{ return minVariance; }
	inline int getShandowMapSizeAsPowerOf2() const{ return shandowMapSizeAsPowerOf2; }

	static inline std::shared_ptr<ShandowInfo> makePtr(ShandowInfo* val){ return std::shared_ptr<ShandowInfo>(val); }

private:
	Matrix4 projection;
	
	bool flipFaces;
	float shandowSoftness;
	float lightBleedReduction;
	float minVariance;
	int shandowMapSizeAsPowerOf2;

};



#define BASELIGHTH
class BaseLight : public GameComponent, std::enable_shared_from_this<BaseLight>{
public:
	BaseLight(const Vector3& color, float intensity);
	virtual ~BaseLight();
	inline float getIntensity() const { return intensity; }
	inline void setIntensity(float val) { intensity = val; }

	inline Vector3 getColor() const { return color; }
	inline void setColor(const Vector3& val) { color = val; }

	
	inline Shader* getShader() { return this->shader.get(); }
	inline std::shared_ptr<ShandowInfo> getShandowInfo(){ return shandowInfo; }
	virtual void addToEngine(CoreEngine* engine) override;

protected:
	inline void setShader(std::shared_ptr<Shader> shader){ this->shader = shader; }

	void setShandowInfo(std::shared_ptr<ShandowInfo> shandowInfo);


private:

	std::shared_ptr<ShandowInfo> shandowInfo;
	Vector3 color;
	float intensity;
	std::shared_ptr<Shader> shader;
};