#ifndef ANIMATION_SCENE_3_H
#define ANIMATION_SCENE_3_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"

class AnimationScene3 : public GameScene
{
private:
	//Time
	float m_curTime;
	float m_preTime;

	//Shaders
	//Basic PBR shader
	Shader m_basicPBRVertexShader;
	Shader m_basicPBRFragShader;

	//Effects
	//Basic phong lighting effect
	Effect m_basicPBREffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Face


	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();


public:
	AnimationScene3() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);

};

#endif