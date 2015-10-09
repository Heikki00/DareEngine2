#include "Mesh.h"
#include "Time.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "assimp\material.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<MeshRecource>> Mesh::loadedModels;


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string& name){
	
	this->filename = name;
	
	if (loadedModels.find(filename) != loadedModels.end() && name != NO_SRC_FILE){

		resource = loadedModels[filename];
	}
	else{

		resource = MeshRecource::makePtr(new MeshRecource(vertices, indices));
		
		loadedModels[filename] = resource;

	}


	
	//calcNorm(vertices, indices);
	

}

Mesh::Mesh(std::string filename)
{
	this->filename = filename;
	





	
	if (loadedModels.find(filename) != loadedModels.end()){
		
		resource = loadedModels[filename];
	}
	else{
		
		
		loadMesh(filename);
		loadedModels[filename] = resource;

	}


	




}


Mesh::~Mesh(){
	if (resource.use_count() == 2){
		loadedModels.erase(filename);
	}
}

void Mesh::loadMesh(std::string filename){

	if (filename.substr(filename.length() - 4, 4) != ".obj"){
		DebugUtility::log_err("ERROR: Only OBJ files are CURRENTLY \"ahem\" supported\n");
		return;
	}



	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);


	
	if (!scene){
		DebugUtility::log_err("ERROR: Failed to load mesh from file: %s\n", filename.c_str());
	}
	
	const aiMesh* model = scene->mMeshes[0];
	
	



	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < model->mNumVertices; i++){
		const aiVector3D* pPos = &(model->mVertices[i]);
		const aiVector3D* pNormal = &(model->mNormals[i]);
		const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
		const aiVector3D* pTangent = &(model->mTangents[i]);
		

		Vertex vert(
			Vector3(pPos->x, pPos->y, pPos->z),
			Vector3(pTexCoord->x, pTexCoord->y, pTexCoord->z),
			Vector3(pNormal->x, pNormal->y, pNormal->z),
			Vector3(pTangent->x, pTangent->y, pTangent->z)
			);

		vertices.push_back(vert);





	}


	for (unsigned int i = 0; i < model->mNumFaces; i++)
	{
		const aiFace& face = model->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}



	resource = MeshRecource::makePtr(new MeshRecource(vertices, indices));





}




void Mesh::draw(){
	glBindVertexArray(resource->getVao());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->getIbo());
	
	glDrawElements(GL_TRIANGLES, resource->getSize(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::calcNorm(std::vector<Vertex>& verticies, std::vector<unsigned int>& indicies){
	
	for (int i = 0; i < indicies.size(); i += 3){
		int i0 = indicies[i];
		int i1 = indicies[i + 1];
		int i2 = indicies[i + 2];

		Vector3 v1 = verticies.at(i1).getPos() - verticies.at(i0).getPos();
		Vector3 v2 = verticies.at(i2).getPos() - verticies.at(i0).getPos();

		Vector3 normal = v1.cross(v2).normalized();

		verticies[i0].setNormal(verticies[i0].getNormal() + normal);
		verticies[i1].setNormal(verticies[i1].getNormal() + normal);
		verticies[i2].setNormal(verticies[i2].getNormal() + normal);
	}

	for (int i = 0; i < verticies.size(); i++){
		verticies.at(i).setNormal(verticies.at(i).getNormal().normalized());
	}


}














