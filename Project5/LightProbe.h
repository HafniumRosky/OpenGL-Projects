#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "Skybox.h"

enum LightProbeVolume
{
	SPHERICAL,
	BOXBASED
};

class LightProbe : public Skybox
{
private:
	std::string m_lightPorbeName;
	std::string m_sceneName;
	bool m_isGlobal;
	bool m_hasDualParaMap = false;
	LightProbeVolume m_volumeShape;

	//Spherical Volume Properties
	float m_volumeRadius;

	//Box-based Volume Properties
	float m_volumeLength;
	float m_volumeWidth;
	float m_volumeHeight;

	//Dual-prarboloid map
	Texture m_dualParaMap[2];
	Texture m_irradianceMap;

	void GeneratePlane(MeshData& texturePlane);
	void GenerateDualParaMap(GLsizei& width, GLsizei& height, MeshData* texturePlane = nullptr);
	void GenerateLUT(GLsizei& size, MeshData* texturePlane = nullptr);
public:
	LightProbe() = default;
	LightProbe(VertexType type, std::string lightProbeName, std::string sceneName, bool isGlobal, std::string skyboxName = "")
	{
		m_type = type;
		m_lightPorbeName = lightProbeName;
		m_sceneName = sceneName;
		m_skyboxName = skyboxName;
		m_isGlobal = isGlobal;
	}

	LightProbe(VertexType type, std::string lightProbeName, std::string sceneName, bool isGlobal, LightProbeVolume volume = SPHERICAL)
	{
		m_type = type;
		m_lightPorbeName = lightProbeName;
		m_sceneName = sceneName;
		m_isGlobal = isGlobal;
		m_volumeShape = volume;
	}

	void LoadLightProbe(GLsizei irrWidth, GLsizei irrHeight, GLsizei prefilterSize, GLsizei LUTSize);
	void LoadLocalLightProbe(GLsizei irrWidth, GLsizei irrHeight, GLsizei prefilterSize, GLsizei LUTSize);
	void SampleCube(float face);
	void GenerateIrradianceMap(int numAzimuth, int numZenith, GLuint cubeID);
	void Prefilter(float roughness, GLuint cubeMapID);
	void IntegrateBRDF();

	//SetVolume
	void SetVolume(float radius);
	void SetVolume(float length, float width, float height);

	float GetLength()
	{
		return m_volumeLength;
	}

	float GetWidth()
	{
		return m_volumeWidth;
	}

	float GetHeight()
	{
		return m_volumeHeight;
	}

	//Compute Weight
	float ComputeWeight(vec3 pos);

	std::string getProbeName()
	{
		return m_lightPorbeName;
	}

	bool HasDualParaMap()
	{
		return m_hasDualParaMap;
	}

	void SetDualParaMap(int index, Texture tex)
	{
		m_dualParaMap[index] = tex;
	}

	void SetIrradianceMap(Texture tex)
	{
		m_irradianceMap = tex;
	}
};

#endif