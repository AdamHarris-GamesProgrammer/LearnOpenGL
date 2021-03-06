#pragma once
#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char* path) {
		LoadModel(path);
	}
	void Draw(Shader* _shader);

	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
private:

	std::string directory;


	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

