#pragma once

#include "Texture.h"
#include <iostream>
#include <memory>
#include "DebugUtility.h"
#include "Utility.h"
#include <unordered_map>
#include "MappedValues.h"




class Material : public MappedValues
{
public:
	
	




	Material();
	

	
	~Material();
	
	void addDisplacementValues(float scale, float offset, std::shared_ptr<Texture> texture = nullptr);

	

	


	inline static std::shared_ptr<Material> makePtr(Material* material){ return  std::shared_ptr<Material>(material); }
	inline static std::shared_ptr<Material> makePtr(){ return  std::shared_ptr<Material>(new Material); }


private:
	
	


	
	



};








