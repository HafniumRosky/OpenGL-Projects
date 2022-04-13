#ifndef ARROW_H
#define ARROW_H

#include "GameObject.h"

class Arrow : public GameObject
{
private:
	//Sphere properties
	float m_radius;
	float m_height;
	unsigned int m_slices;
	vec4 m_color;

	void GenerateLine(MeshData& lineMesh);
	void GenerateCone(MeshData& coneMesh, float radius, float height, unsigned int slices);
public:
	Arrow() = default;
	Arrow(VertexType type, float radius, float height, unsigned int slices, vec4 color)
	{
		m_type = type;
		m_radius = radius;
		m_height = height;
		m_slices = slices;
		m_color = color;
	}
	~Arrow() {}
	void LoadGameObject();
	void Draw();
	void Update(float dt);
	void SetColor(vec4 color);
	void ResetColor();
};

#endif