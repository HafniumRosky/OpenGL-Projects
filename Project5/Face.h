#ifndef FACE_H
#define FACE_H

#include "GameObject.h"
#include <io.h>
#include <sstream>

class Face : public GameObject
{
private:
	std::string m_modelPath;
	std::string m_textureDir;
	std::string m_modelFolder;
	std::string m_neutralName;
	std::vector<std::string> m_blendShapeFiles;

	std::vector<float> m_blendWeights;
	unsigned int m_neutralIndex;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	void GetFiles(std::string path, std::vector<std::string>& files);
	void GenerateBuffers();

public:
	Face() = default;
	Face(VertexType type, std::string modelDirectory, std::string modelName, bool blendShape = false)
	{
		m_type = type;
		m_blendShape = blendShape;
		m_modelPath = "../Project5\\Assets\\Models\\" + modelDirectory + "\\" + modelName;
		m_modelFolder = "../Project5\\Assets\\Models\\" + modelDirectory;
		m_textureDir = "../Project5\\Assets\\Textures\\" + modelDirectory;
		m_neutralName = modelName;
	}

	void SetEffect(Effect* pEffect);
	void Assemble();
	void LoadGameObject();
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void DrawPBR();
	void DrawPBRBlend();
	void Update(float dt);

	std::vector<float> GetBlendWeight()
	{
		return m_blendWeights;
	}

	void SetBlendWeight(int index, float value)
	{
		m_blendWeights[index] = value;
	}

	unsigned int getNeutralIndex()
	{
		return m_neutralIndex;
	}
};

#endif