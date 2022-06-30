#pragma once


#include <iostream>;
#include "mesh.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>

#include "engine.h"
#include "shader.h"


namespace gpr5300
{
class ModelObject
{
public:
	std::vector<Texture> textures_loaded;
	glm::mat4 model = glm::mat4(1.0);
	void InitModel(const char* path);
	void Draw(const Shader& pipeline) const;
	void MultipleDraw(const Shader& pipeline, int amount) const;
	//void MeshSetup() ;
	
	std::vector<Mesh> meshes;
	Mesh mesh_;
private:

	
	std::string directory;
	
	void loadModel(const std::string& path);
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
};
	
}
