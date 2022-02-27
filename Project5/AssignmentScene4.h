#ifndef ASSIGNMENT_SCENE4_H
#define ASSIGNMENT_SCENE4_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Plane.h"
#include "imgui.h"

class AssignmentScene4 : public GameScene
{
private:
	//Time
	float m_curTime;
	float m_preTime;

	float m_renderStateTimer = 0.0f;
	bool m_useMipMap = false;

	//Shaders
	//Texture debug shader
	Shader m_textureDebugVertexShader;
	Shader m_textureDebugFragShader;

	//Effects
	//Texture debug effect
	Effect m_textureDebugEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Plane
	Plane m_plane;

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

public:
	AssignmentScene4() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
	void GUIDisplay();
};

#endif