#ifndef BASIC_SCENE_H
#define BASIC_SCENE_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Skybox.h"
#include "Ship.h"
#include "Plane.h"
#include "ParticleSystem.h"
#include "Ball.h"

class BasicScene : public GameScene
{
private:
	GLsizei m_sceneWidth = 1920;
	GLsizei m_sceneHeight = 1080;
	//Shaders
	//Basic lighting shader
	Shader m_shipVertexShader;
	Shader m_shipFragShader;
	//Skybox shader
	Shader m_skyboxVertexShader;
	Shader m_skyboxFragShader;
	//Ocean shader
	Shader m_oceanVertexShader;
	Shader m_oceanHullShader;
	Shader m_oceanDomainShader;
	Shader m_oceanFragShader;
	Shader m_oceanComputeShader;
	//Smoke shader
	Shader m_smokeUpdateVertexShader;
	Shader m_smokeUpdateGeometryShader;
	Shader m_smokeRenderVertexShader;
	Shader m_smokeRenderGeometryShader;
	Shader m_smokeRenderFragShader;
	//Cannon ball shader
	Shader m_ballVertexShader;
	Shader m_ballFragShader;

	//General purpose shader
	Shader m_depthInitiShader;
	Shader m_depthComputeShader;

	//Effects
	//Basic ship lighting effect
	Effect m_shipEffect;
	//Skybox rendering effect
	Effect m_skyboxEffect;
	//Ocean rendering effect
	Effect m_oceanEffect;
	//Ocean wave effect
	Effect m_oceanWaveEffect;
	//Depth initializing effect
	Effect m_depthInitiEffect;
	//Ship floating effect
	Effect m_shipFloatEffect;
	//Smoke update effect
	Effect m_smokeUpdateEffect;
	//Smoke render effect
	Effect m_smokeRenderEffect;
	//Cannon ball rendering effect
	Effect m_ballEffect;

	//Lights
	//Directional light
	std::vector<DirectionalLight> m_directLight;
	std::vector<PointLight> m_pointLight;

	//Game Objects
	//Skybox
	Skybox m_skybox;
	//Player ship
	Ship m_playerShip;
	//Hostile ship
	Ship m_hostileShip;
	//Ocean
	Plane m_ocean;
	//Smoke
	ParticleSystem m_smoke;
	//Cannon balls
	std::vector<Ball> m_rounds;

	//Shadow Map
	RenderTexture m_shadowMap;
	//Size
	GLsizei m_shadowMapWidth = 8192;
	GLsizei m_shadowMapHeight = 8192;
	//Matrix
	mat4 m_shadowMatrix;

	//Image data
	//Size
	GLsizei m_depthSize = 32;
	//Player ship depth
	GLfloat* m_playerShipDepth;
	//Player ocean depth
	GLfloat* m_playerOceanDepth;
	//Hostile ship depth
	GLfloat* m_hostileShipDepth;
	//Hostile ocean depth
	GLfloat* m_hostileOceanDepth;

	//Cameras
	Camera m_camera;
	Camera m_depthCamera;
	Camera m_shadowCamera;

	//Camera State
	bool m_isCaptain = true;

	//Timer
	int m_curTime = 0;
	int m_preTime = 0;

	//Draw calls
	void RenderDepth();
	void RenderShadows();
	void RenderObjects();
	//FBO data fetch
	void FetchData(RenderTexture rt, GLfloat* data);
	void FetchData(RenderTexture rt, GLint* data);
public:
	BasicScene() 
	{
		m_playerShipDepth = new GLfloat[1024];
		m_playerOceanDepth = new GLfloat[1024];
		m_hostileShipDepth = new GLfloat[1024];
		m_hostileOceanDepth = new GLfloat[1024];
	}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
};


#endif