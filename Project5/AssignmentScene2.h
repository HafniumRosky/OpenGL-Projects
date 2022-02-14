#ifndef ASSIGNMENT_SCENE_2_H
#define ASSIGNMENT_SCENE_2_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Skybox.h"
#include "Ball.h"
#include "Dragon.h"

class AssignmentScene2 : public GameScene
{
private:
	GLsizei m_sceneWidth = 1920;
	GLsizei m_sceneHeight = 1080;

	int m_renderState = 0;
	float m_renderStateTimer = 0.0f;
	float m_etas[4] = { 0.66f, 0.62f, 0.67f, 0.73f };
	int m_renderTarget = 0;
	float m_renderTargetTimer = 0.0f;

	//Time
	float m_curTime;
	float m_preTime;

	//Shaders
	//Basic PBR shader
	Shader m_perfectSurfaceVertexShader;
	Shader m_perfectSurfaceFragShader;
	//HDR sampler shader
	Shader m_HDRSamplerVertexShader;
	Shader m_HDRSamplerFragShader;
	//HDR environment map shader
	Shader m_HDREnvMapVertexShader;
	Shader m_HDREnvMapFragShader;

	//Effects
	//Basic PBR effect
	Effect m_perfectSurfaceEffect;
	//HDR sampler effect
	Effect m_HDRSamplerEffect;
	//HDR environment map effect
	Effect m_HDREnvMapEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//PBR material sphere
	Ball m_sphere;
	//Dragon
	Dragon m_dragon;
	//HDR environment
	Skybox m_HDREnvir;

	//Cameras
	Camera m_camera;
	Camera m_samplerCam;

	//Draw call
	void RenderObjects();

	//Precomputation
	void SampleHDR();

public:
	AssignmentScene2() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};

#endif