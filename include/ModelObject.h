#pragma once


#include <iostream>;
#include "mesh.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>


namespace gpr5300
{
   
    class ModelObject
    {
    public:
        
        void InitModel(const char* path);
        unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
        void Draw(Pipeline& pipeline);
    private:
        // model data
        
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
    };
}