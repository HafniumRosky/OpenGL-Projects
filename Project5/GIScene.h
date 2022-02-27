#ifndef GI_SCENE_H
#define GI_SCENE_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Ball.h"
#include "Skybox.h"
#include "LightProbe.h"

class GIScene : public GameScene
{
private:
	unsigned int m_maxMipLevel = 5;

	GLsizei m_prefilterResolution = 128;

	GLsizei m_sceneWidth = 1920;
	GLsizei m_sceneHeight = 1080;

	//Time
	float m_curTime;
	float m_preTime;

	//Shaders
	//Basic PBR shader
	Shader m_basicPBRVertexShader;
	Shader m_basicPBRFragShader;
	//Diffuse irradiance shader
	Shader m_diffuseIrradianceVertexShader;
	Shader m_diffuseIrradianceFragShader;
	//Prefilter map shader
	Shader m_prefilterMapVertexShader;
	Shader m_prefilterMapFragShader;
	//HDR sampler shader
	Shader m_HDRSamplerVertexShader;
	Shader m_HDRSamplerFragShader;
	//HDR environment map shader
	Shader m_HDREnvMapVertexShader;
	Shader m_HDREnvMapFragShader;
	//Dual-paraboloid generator shader
	Shader m_DualParaGenVertexShader;
	Shader m_DualParaGenFragShader;
	//GI baking shader
	Shader m_bakeGIVertexShader;
	Shader m_bakeGIFragShader;

	//Effects
	//Basic PBR effect
	Effect m_basicPBREffect;
	//Diffuse Irradiance effect
	Effect m_diffuseIrradianceEffect;
	//Prefilter map effect
	Effect m_prefilterMapEffect;
	//HDR sampler effect
	Effect m_HDRSamplerEffect;
	//HDR environment map effect
	Effect m_HDREnvMapEffect;
	//Dual-paraboloid generator effect
	Effect m_DualParaGenEffect;
	//GI baking effect
	Effect m_bakeGIEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;


	//GameObjects
	//PBR material sphere
	Ball m_sphere;
	//HDR environment
	LightProbe m_HDREnvir;
	GLsizei m_HDRCubeWidth = 512;
	GLsizei m_HDRCubeHeight = 512;

	//Shadow Map
	RenderTexture m_shadowMap;
	//Size
	GLsizei m_shadowMapWidth = 8192;
	GLsizei m_shadowMapHeight = 8192;
	//Matrix
	mat4 m_shadowMatrix;

	//Cameras
	Camera m_camera;
	Camera m_samplerCam;

	//Draw Calls
	void RenderShadows();
	void RenderObjects();

	//Precomputation
	void SampleHDR();
	void TransformCubeToDualPara();
	void BakeIrradianceMap();
	void BakePrefilteredMap();
	void Bake();

public:
	GIScene() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);

};

#endif