#ifndef SKYBOX_H
#define SKYBOX_H

#include "GameObject.h"

class Skybox : public GameObject
{
protected:
	std::string m_skyboxName;
	std::string m_textureFormat;

	void GenerateCube(MeshData& skyboxMesh);
	void SetCubeMap(std::string skyboxName, std::string textureFormat, MeshData& skyboxMesh);
	void LoadHDR(MeshData& skyboxMesh);
	void GenerateEmptyCubeMap(MeshData& skyboxMesh, GLsizei width, GLsizei height, bool mip = false);

public:
	Skybox() = default;
	Skybox(VertexType type, std::string skyboxName, std::string textureFormat)
	{
		m_type = type;
		m_skyboxName = skyboxName;
		m_textureFormat = textureFormat;
	}

	void LoadGameObject();
	void LoadHDRSampler(GLsizei width, GLsizei height);
	void SampleHDR();
	void Draw(int texIndex);

	void BindVertexBuffer()
	{
		m_meshVec[0].BindBuffers();
	}
};

#endif