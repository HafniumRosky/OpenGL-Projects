#include "Arrow.h"

void Arrow::GenerateLine(MeshData& lineMesh)
{
	lineMesh.m_posL.push_back(vec3(0.0f));
	lineMesh.m_posL.push_back(vec3(1.0f, 0.0f, 0.0f));
	lineMesh.m_indexVec.push_back(0);
	lineMesh.m_indexVec.push_back(1);
}

void Arrow::GenerateCone(MeshData& coneMesh, float radius, float height, unsigned int slices)
{
	float h2 = height / 2.0f;
	float theta = 0.0f;
	float perTheta = 2 * 3.1415926 / slices;
	unsigned int iIndex = 3 * slices;
	unsigned int vIndex = 2 * slices;

	//Vertex data
	//Top
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = i * perTheta + perTheta / 2;
		coneMesh.m_posL.push_back(vec3(0.0f, h2, 0.0f));
	}

	//Bottom of side
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = i * perTheta;
		coneMesh.m_posL.push_back(vec3(radius * glm::cos(theta), -h2, radius * glm::sin(theta)));
	}

	//Bottom
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = i * perTheta;
		coneMesh.m_posL.push_back(vec3(radius * glm::cos(theta), -h2, radius * glm::sin(theta)));
	}

	//Bottom center
	coneMesh.m_posL.push_back(vec3(0.0f, -h2, 0.0f));

	//Index data
	for (unsigned int i = 0; i < slices; ++i)
	{
		coneMesh.m_indexVec.push_back(i);
		coneMesh.m_indexVec.push_back(slices + (i + 1) % slices);
		coneMesh.m_indexVec.push_back(slices + i % slices);
	}
	unsigned int offset = 2 * slices;
	for (unsigned int i = 0; i < slices; ++i)
	{
		coneMesh.m_indexVec.push_back(offset + slices);
		coneMesh.m_indexVec.push_back(offset + i % slices);
		coneMesh.m_indexVec.push_back(offset + (i + 1) % slices);
	}
	coneMesh.m_childTransform.SetRotation(vec3(0.0f, 0.0f, -radians(90.0f)));
	coneMesh.m_childTransform.SetPosition(vec3(1.0f, 0.0f, 0.0f));
}

void Arrow::LoadGameObject()
{
	MeshData lineMesh;
	lineMesh.SetVertexType(m_type);
	GenerateLine(lineMesh);
	lineMesh.m_material.diffuse = m_color;
	m_meshVec.push_back(lineMesh);//mesh 0

	MeshData coneMesh;
	coneMesh.SetVertexType(m_type);
	GenerateCone(coneMesh, m_radius, m_height, m_slices);
	coneMesh.m_material.diffuse = m_color;
	m_meshVec.push_back(coneMesh);//mesh 1
}

void Arrow::SetColor(vec4 color)
{
	m_meshVec[0].m_material.diffuse = color;
	m_meshVec[1].m_material.diffuse = color;
}

void Arrow::ResetColor()
{
	m_meshVec[0].m_material.diffuse = m_color;
	m_meshVec[1].m_material.diffuse = m_color;
}

void Arrow::Draw()
{
	//Line
	this->GetTransform().ComputeLocalToWorldMatrix();
	//Bind effect
	m_meshVec[0].m_pEffect->BindEffect();
	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDraw.world = GetMeshWorld(0);
	//World inverse matrix
	Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(0);
	//Materials
	Effect::cbDraw.material = m_meshVec[0].m_material;
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[0].BindVertexArray();
	glDrawElements(GL_LINE_STRIP, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);

	//Cone
	this->GetTransform().ComputeLocalToWorldMatrix();
	//Bind effect
	m_meshVec[1].m_pEffect->BindEffect();
	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDraw.world = GetMeshWorld(1);
	//World inverse matrix
	Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(1);
	//Materials
	Effect::cbDraw.material = m_meshVec[1].m_material;
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[1].m_pEffect->m_UBOid[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[1].BindVertexArray();
	glDrawElements(GL_TRIANGLES, m_meshVec[1].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Arrow::Update(float dt)
{

}