#ifndef ANIMATION_SCENE_1_H
#define ANIMATION_SCENE_1_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "AirPlane.h"
#include "Skybox.h"

class AnimationScene1 : public GameScene
{
private:
	//Time
	float m_curTime;
	float m_preTime;
	float m_camStateTimer = 0.0f;

	//Shaders
	//Basic phong shader
	Shader m_phongVertexShader;
	Shader m_phongFragShader;
	//Skybox shader
	Shader m_skyboxVertexShader;
	Shader m_skyboxFragShader;

	//Effects
	//Basic phong lighting effect
	Effect m_phongEffect;
	//Skybox rendering effect
	Effect m_skyboxEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Plane
	AirPlane m_airplane;
	//Skybox
	Skybox m_skybox;

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

public:
	AnimationScene1() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};

#endif