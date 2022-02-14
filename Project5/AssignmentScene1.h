#ifndef ASSIGNMENT_SCENE_1_H
#define ASSIGNMENT_SCENE_1_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Cube.h"
#include "Ball.h"

class AssignmentScene1 : public GameScene
{
private:
	bool m_renderCube = false;
	//Time
	float m_curTime;
	float m_preTime;

	//Shaders
	//Basic PBR shader
	Shader m_basicPBRVertexShader;
	Shader m_basicPBRFragShader;
	//Basic phong shader
	Shader m_phongVertexShader;
	Shader m_phongFragShader;
	//Anime shader
	Shader m_animeVertexShader;
	Shader m_animeFragShader;
	//OutLine shader
	Shader m_outlineVertexShader;
	Shader m_outlineFragShader;

	//Effects
	//Basic PBR effect
	Effect m_basicPBREffect;
	//Basic phong lighting effect
	Effect m_phongEffect;
	//Anime effect
	Effect m_animeEffect;
	//OutLine effect
	Effect m_outlineEffect;


	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;
	bool m_lightMovingRight = true;

	//GameObjects
	//PBR material Cube
	Cube m_Cube[35];
	//PBR material Sphere
	Ball m_Sphere[35];

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

public:
	AssignmentScene1() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};

#endif