#ifndef CUBE_H
#define CUBE_H

#include "GameObject.h"

class Cube : public GameObject
{
private:
	bool m_loadNormal = false;
	bool m_loadAO = false;
	std::string m_texturePath;
	std::string m_materialName;
	
	glm::vec4 m_emmision = vec4(0.0f);

	void GenerateCube(MeshData& cubeMesh);
	void LoadPBRTextures(MeshData& cubeMesh, bool loadTex);

public:
	Cube() = default;
	Cube(VertexType type, std::string path = " ",  bool loadNormal = false, bool loadAO = false)
	{
		m_type = type;
		m_loadNormal = loadNormal;
		m_texturePath = "../Project5\\Assets\\Textures\\" + path;
		m_materialName = path;
		m_loadAO = loadAO;
	}
	void LoadGameObject(float gloss);
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void Draw();
	void DrawPBR();
	void Update(float dt);

	void PushIrradianceMap(Texture irrTex)
	{
		m_meshVec[0].m_TextureVec.push_back(irrTex);
	}

	void PushPrefilteredMap(Texture prefilteredTex)
	{
		m_meshVec[0].m_TextureVec.push_back(prefilteredTex);
	}

	void PushLUT(Texture LUT)
	{
		m_meshVec[0].m_TextureVec.push_back(LUT);
	}
};

#endif