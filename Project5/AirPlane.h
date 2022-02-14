#ifndef AIR_PLANE_H
#define AIR_PLANE_H

#include "GameObject.h"

class AirPlane : public GameObject
{
private:
	std::string m_modelPath;
	std::string m_textureDir;

	bool m_isDiving;
	bool m_isRollingCW;
	bool m_isTruningRight;

	float m_stateChangeTimer = 0.0f;

	void Fly(float dt);
	void RollPropeller(float dt);

public:
	AirPlane() = default;
	AirPlane(VertexType type, std::string modelDirectory, std::string modelName)
	{
		m_type = type;
		m_modelPath = "../Project5\\Assets\\Models\\" + modelDirectory + "\\" + modelName;
		m_textureDir = "../Project5\\Assets\\Textures\\" + modelDirectory;
	}

	void LoadGameObject();
	void Draw();
	void Update(float dt);
};

#endif