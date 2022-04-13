#ifndef PLANE_H
#define PLANE_H

#include "GameObject.h"

class Plane : public GameObject
{
private:
	int m_planeVertWidth = 200;
	int m_planeHorWidth = 200;
	float m_textureScale = 1.0f;
	GLuint m_environmentMapID;

	bool m_loadNormal;
	bool m_loadAO;
	std::string m_texturePath;
	std::string m_materialName;

	glm::vec4 m_emmision = glm::vec4(0.0f);

	void GeneratePlane(MeshData& planeMesh);
	void GenerateTexture(MeshData& planeMesh, GLsizei width, GLsizei height);
	void GenerateDubugTexture(MeshData& planeMesh, GLsizei width, GLsizei height);
	void LoadPBRTextures(MeshData& planeMesh, bool loadTex);

public:
	float m_probeWeight[2];
	glm::vec3 m_probePos[2];

	Plane() = default;
	Plane(VertexType type, int vert, int hor, std::string path = " ", bool loadNoraml = false, bool loadAO = false)
	{
		m_type = type;
		m_planeVertWidth = vert;
		m_planeHorWidth = hor;
		m_texturePath = "../Project5\\Assets\\Textures\\" + path;
		m_materialName = path;
		m_loadNormal = loadNoraml;
		m_loadAO = loadAO;
	}
	void LoadGameObject(GLsizei texWidth, GLsizei texHeight);
	void LoadDebugObject(GLsizei texWidth, GLsizei texHeight);
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f, float ao = -1.0f, vec4 emmisive = vec4(0.0f));
	void Draw();
	void DrawPBR();
	void DrawDebug();
	void Update(float deltaTime);
	void SetEnvironmentMapID(GLuint cubeID)
	{
		m_environmentMapID = cubeID;
	}

	void SetTextureScale(float scale)
	{
		m_textureScale = scale;
	}

	//for test
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