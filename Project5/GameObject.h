#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm.hpp>
#include <vector>
#include <map>
#include <stb_image.h>
#include <string>
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"

extern std::vector<Texture> loadedTexture;

//This class defines all the objects in the scene and contains all the properties.
class GameObject
{
private:
	Transform m_transform;

	//This function will load mesh from the aiScene by recursively calling itself, all the meshes of this object can be loaded by calling this function with root node
	void LoadMeshFromScene(const aiNode* pCurrentNode, const aiScene* pAIScene, std::string textureDir);

	//This function will load mesh data into mesh vector which index matches the index in the scene
	void LoadMeshData(const aiMesh* pMesh, unsigned int index, const aiScene* pAIScene, std::string textureDir);

	//This function will load mesh texture into mesh's texture vector
	void LoadMeshTexture(aiMaterial* mat, aiTextureType type, std::string typeName, unsigned int index, std::string textureDir);

protected:
	//Vertex
	std::vector<MeshData> m_meshVec;
	GLuint m_verticesCount = 0;
	VertexType m_type;

	//ShadowMap
	GLuint m_shadowMapID;

	//This function will load all properties into the aiScene pointer
	void LoadGameObjectFromFile(std::string modelPath, std::string textureDir);

	//Load Textures
	void SetUpTextureMapFromFile(const std::string directory, bool mipmap, Texture* pTexture, GLint texClamp = GL_REPEAT, int* pNumColorChannels = NULL);

public:
	GameObject() = default;

	//Assemble the input data into GPU
	void InputAssemble();
	void InputAssemble(int index);

	void virtual LoadGameObject() {}
	void virtual Draw() {}

	//Accessors

	// This function will acess transform property of the object
	Transform& GetTransform()
	{
		return m_transform;
	}
	// This function will get transform property of the object
	const Transform& GetTransform() const
	{
		return m_transform;
	}

	mat4 GetWorldMatrix()
	{
		return m_transform.GetLoaclToWorldMatrix();
	}

	int GetMeshCount()
	{
		return this->m_meshVec.size();
	}

	void SetMeshEffectWithIndex(Effect* pEffect, int meshIndex)
	{
		m_meshVec[meshIndex].m_pEffect = pEffect;
	}

	mat4 GetMeshWorld(int index)
	{
		m_meshVec[index].m_childTransform.ComputeLocalToWorldMatrix();
		return m_transform.GetLoaclToWorldMatrix() * m_meshVec[index].m_childTransform.GetLoaclToWorldMatrix();
	}

	mat4 GetMeshInverseTranspose(int index)
	{
		mat4 invWorld = GetMeshWorld(index);
		invWorld = inverse(invWorld);
		return transpose(invWorld);
	}

	GLuint GetMeshTextureID(int index, int texIndex)
	{
		return m_meshVec[index].m_TextureVec[texIndex].texID;
	}

	Texture GetMeshTexture(int index, int texIndex)
	{
		return m_meshVec[index].m_TextureVec[texIndex];
	}

	void SetShadowMap(GLuint texID)
	{
		m_shadowMapID = texID;
	}
};
#endif