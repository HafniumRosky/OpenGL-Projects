#ifndef BALL_H
#define BALL_H

#include "GameObject.h"

class Ball : public GameObject
{
private:
	//Sphere properties
	float m_radius;
	unsigned int m_levels;
	unsigned int m_slices;
	std::string m_texturePath;
	std::string m_materialName;
	//Cannon ball properties
	float m_velocity = 50.0f;
	vec3 m_direction;
	bool m_loadNormal;

	vec4 m_color;
	void GenerateSphere(MeshData& sphereMesh, float radius, unsigned int levels, unsigned int slices, vec4 color);
	void LoadPBRTextures(MeshData& sphereMesh, bool loadTex);
public:
	Ball() = default;
	Ball(VertexType type, float radius, unsigned int levels, unsigned int slices, const vec4 color, std::string path = " ", bool loadNormal = false)
	{
		m_type = type;
		m_radius = radius;
		m_levels = levels;
		m_slices = slices;
		m_color = vec4(color.x, color.y, color.z, color.a);
		m_texturePath = "../Project5\\Assets\\Textures\\" + path;
		m_materialName = path;
		m_loadNormal = loadNormal;
	}
	~Ball() {}
	void LoadGameObject(float gloss);
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void Draw();
	void DrawPBR();
	void DrawPerfect(GLuint cubeTexID, int renderState, float* etas);
	void Update(float dt);

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

	void SetDirection(vec3 dir)
	{
		m_direction = dir;
	}
};

#endif