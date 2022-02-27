#ifndef PLANE_H
#define PLANE_H

#include "GameObject.h"

class Plane : public GameObject
{
private:
	int m_planeVertWidth = 200;
	int m_planeHorWidth = 200;
	float m_textureScale = 1.0f;
	GLuint m_environmentMapID;
	void GeneratePlane(MeshData& planeMesh);
	void GenerateTexture(MeshData& planeMesh, GLsizei width, GLsizei height);
	void GenerateDubugTexture(MeshData& planeMesh, GLsizei width, GLsizei height);

public:
	Plane() = default;
	Plane(VertexType type, int vert, int hor)
	{
		m_type = type;
		m_planeVertWidth = vert;
		m_planeHorWidth = hor;
	}
	void LoadGameObject(GLsizei texWidth, GLsizei texHeight);
	void LoadDebugObject(GLsizei texWidth, GLsizei texHeight);
	void Draw();
	void DrawDebug();
	void Update(float deltaTime);
	void SetEnvironmentMapID(GLuint cubeID)
	{
		m_environmentMapID = cubeID;
	}

	void SetTextureScale(float scale)
	{
		m_textureScale = scale;
	}
};

#endif