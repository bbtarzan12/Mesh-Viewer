#include "MeshLoader.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>

void MeshLoader::Load(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& tangents, std::vector<unsigned int>& indices)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	assert(scene);
	assert(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
	assert(scene->mRootNode);

	const aiMesh* mesh = scene->mMeshes[0];

	int numVertices = mesh->mNumVertices;

	vertices.reserve(numVertices);
	for (size_t i = 0; i < numVertices; i++)
	{
		aiVector3D& pos = mesh->mVertices[i];
		vertices.emplace_back(pos.x, pos.y, pos.z);
	}

	uvs.reserve(numVertices);
	for (size_t i = 0 ; i < numVertices; i++)
	{
		aiVector3D& uvw = mesh->mTextureCoords[0][i];
		uvs.emplace_back(uvw.x, uvw.y);
	}

	normals.reserve(numVertices);
	for (size_t i = 0 ; i < numVertices; i++)
	{
		aiVector3D& n = mesh->mNormals[i];
		normals.emplace_back(n.x, n.y, n.z);
	}

	tangents.reserve(numVertices);
	for (size_t i = 0; i < numVertices; i++)
	{
		aiVector3D& t = mesh->mTangents[i];
		tangents.emplace_back(t.x, t.y, t.z);
	}

	indices.reserve(3 * numVertices);
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}
