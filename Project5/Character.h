#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"

class Character : public GameObject
{
private:
	std::string m_modelPath;
	std::string m_textureDir;

public:
	Character() = default;
	Character(VertexType type, std::string modelDirectory, std::string modelName)
	{
		m_type = type;
		m_modelPath = "../Project5\\Assets\\Models\\" + modelDirectory + "\\" + modelName;
		m_textureDir = "../Project5\\Assets\\Textures\\" + modelDirectory;
	}

	void LoadGameObject();
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void DrawPBR();
	void Update(float dt);

};

#endif