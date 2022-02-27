#ifndef ANIMATIONSCENE_2_H
#define ANIMATIONSCENE_2_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Character.h"
#include "Ball.h"

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
	//Basic PBR sphere shader
	Shader m_basicPBRSphereVertexShader;
	Shader m_basicPBRSphereFragShader;

	//Effects
	//Basic phong lighting effect
	Effect m_basicPBREffect;
	//Basic PBR sphere effect
	Effect m_basicPBRSphereEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Character
	Character m_militia;
	Ball m_sphere;
	

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