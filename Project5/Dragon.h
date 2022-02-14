#ifndef DRAGON_H
#define DRAGON_H

#include "GameObject.h"

class Dragon : public GameObject
{
private:
	std::string m_modelPath;
	std::string m_textureDir;

public:
	Dragon() = default;
	Dragon(VertexType type, std::string modelDirectory, std::string modelName)
	{
		m_type = type;
		m_modelPath = "../Project5\\Assets\\Models\\" + modelDirectory + "\\" + modelName;
		m_textureDir = "../Project5\\Assets\\Textures\\" + modelDirectory;
	}

	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void DrawPerfect(GLuint cubeTexID, int renderState, float* etas);
	void Update(float dt);
};

#endif