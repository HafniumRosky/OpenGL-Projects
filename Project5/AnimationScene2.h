#ifndef ANIMATIONSCENE_2_H
#define ANIMATIONSCENE_2_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Character.h"

class AnimationScene2 : public GameScene
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
	//Character
	Character m_militia;

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

public:
	AnimationScene2() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};

#endif