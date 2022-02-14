#ifndef ASSIGNMENTSCENE_3_H
#define ASSIGNMENTSCENE_3_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Character.h"

class AssignmentScene3 : public GameScene
{
private:
	//Time
	float m_curTime;
	float m_preTime;

	float m_renderStateTimer = 0.0f;

	//Shaders
	//Basic PBR shader with normal map
	Shader m_basicPBRTanVertexShader;
	Shader m_basicPBRTanFragShader;
	//Basic PBR shader without normal map
	Shader m_basicPBRVertexShader;
	Shader m_basicPBRFragShader;

	//Effects
	//Basic PBR lighting effect
	Effect m_basicPBRTanEffect;
	Effect m_basicPBREffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Character
	Character m_militia;

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

public:
	AssignmentScene3() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};

#endif