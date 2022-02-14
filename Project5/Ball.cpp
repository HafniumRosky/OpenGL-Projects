#include "Ball.h"

void Ball::GenerateSphere(MeshData& sphereMesh, float radius, unsigned int levels, unsigned int slices, vec4 color)
{
	unsigned int vertexCount = 2 + (levels - 1) * (slices + 1);
	unsigned int indexCount = 6 * (levels - 1) * slices;
	//sphereMesh.m_posL.resize(vertexCount);
	sphereMesh.m_indexVec.resize(indexCount);
	unsigned int vIndex = 0, iIndex = 0;
	float phi = 0.0f, theta = 0.0f;
	float per_phi = 3.1415927 / levels;
	float per_theta = 3.1415927 * 2 / slices;
	float x, y, z;
	sphereMesh.m_posL.push_back(vec3(0.0f, radius, 0.0f));
	sphereMesh.m_normalL.push_back(vec3(0.0f, 1.0f, 0.0f));
	sphereMesh.m_color.push_back(color);
	sphereMesh.m_texcoord.push_back(vec2(0.0f, 0.0f));
	for (unsigned int i = 1; i < levels; ++i)
	{
		phi = per_phi * i;
		for (unsigned int j = 0; j <= slices; ++j)
		{
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			sphereMesh.m_posL.push_back(vec3(x, y, z));
			sphereMesh.m_normalL.push_back(normalize(vec3(x, y, z)));
			sphereMesh.m_color.push_back(color);
			sphereMesh.m_texcoord.push_back(vec2(theta / 2 * 3.1415927, phi / 2 * 3.1415927));
		}
	}
	sphereMesh.m_posL.push_back(vec3(0.0f, -radius, 0.0f));
	sphereMesh.m_normalL.push_back(vec3(0.0f, -1.0f, 0.0f));
	sphereMesh.m_color.push_back(color);
	sphereMesh.m_texcoord.push_back(vec2(0.0f, 1.0f));
	//Index
	if (levels > 1)
	{
		for (unsigned int j = 1; j <= slices; ++j)
		{
			sphereMesh.m_indexVec[iIndex++] = 0;
			sphereMesh.m_indexVec[iIndex++] = j % (slices + 1) + 1;
			sphereMesh.m_indexVec[iIndex++] = j;
		}
	}
	for (unsigned int i = 1; i < levels - 1; ++i)
	{
		for (unsigned int j = 1; j <= slices; ++j)
		{
			sphereMesh.m_indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
			sphereMesh.m_indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
			sphereMesh.m_indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

			sphereMesh.m_indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
			sphereMesh.m_indexVec[iIndex++] = i * (slices + 1) + j;
			sphereMesh.m_indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
		}
	}

	if (levels > 1)
	{
		for (unsigned int j = 1; j <= slices; ++j)
        {
			sphereMesh.m_indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
			sphereMesh.m_indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
			sphereMesh.m_indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
        }
	}
	
}

void Ball::LoadPBRTextures(MeshData& sphereMesh, bool loadTex)
{
	Texture albedoMap;
	albedoMap.name = m_materialName + "_basecolor.png";
	albedoMap.type = "albedo";
	Texture metallicMap;
	metallicMap.name = m_materialName + "_metallic.png";
	metallicMap.type = "metallic";
	Texture roughnessMap;
	roughnessMap.name = m_materialName + "_roughness.png";
	roughnessMap.type = "roughness";


	if (loadTex)
	{
		SetUpTextureMapFromFile(m_texturePath, true, &albedoMap, GL_MIRRORED_REPEAT);
		SetUpTextureMapFromFile(m_texturePath, true, &metallicMap, GL_MIRRORED_REPEAT);
		SetUpTextureMapFromFile(m_texturePath, true, &roughnessMap, GL_MIRRORED_REPEAT);
	}

	sphereMesh.m_TextureVec.push_back(albedoMap);
	sphereMesh.m_TextureVec.push_back(metallicMap);
	sphereMesh.m_TextureVec.push_back(roughnessMap);

	if (m_loadNormal)
	{
		Texture normalMap;
		normalMap.name = m_materialName + "_normal.png";
		normalMap.type = "normal";
		SetUpTextureMapFromFile(m_texturePath, true, &normalMap, GL_MIRRORED_REPEAT);
		sphereMesh.m_TextureVec.push_back(normalMap);
	}
}

void Ball::LoadGameObject(float gloss)
{
	MeshData sphereMesh;
	sphereMesh.SetVertexType(m_type);
	GenerateSphere(sphereMesh, m_radius, m_levels, m_slices, m_color);
	m_meshVec.push_back(sphereMesh);
	m_meshVec[0].m_material.ambient = vec4(0.19225f, 0.19225f, 0.19225f, 1.0f);
	m_meshVec[0].m_material.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshVec[0].m_material.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshVec[0].m_material.gloss = gloss;
}

void Ball::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
	MeshData sphereMesh;
	sphereMesh.SetVertexType(m_type);
	sphereMesh.m_PBRMaterial.albedo = albedo;
	sphereMesh.m_PBRMaterial.metallic = metallic;
	sphereMesh.m_PBRMaterial.roughness = roughness;
	GenerateSphere(sphereMesh, m_radius, m_levels, m_slices, m_color);
	bool loadTex = true;
	if (albedo.x >= 0)
		loadTex = false;
	LoadPBRTextures(sphereMesh, loadTex);
	m_meshVec.push_back(sphereMesh);
}

void Ball::Draw()
{
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
	glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Ball::DrawPBR()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	//Bind effect
	m_meshVec[0].m_pEffect->BindEffect();
	//Bind texture
	std::vector<GLuint> texIDVec;
	std::vector<const char*> texParaNameVec;
	std::vector<GLenum> texTarget;
	texIDVec.push_back(m_meshVec[0].m_TextureVec[0].texID);
	texIDVec.push_back(m_meshVec[0].m_TextureVec[1].texID);
	texIDVec.push_back(m_meshVec[0].m_TextureVec[2].texID);
	texParaNameVec.push_back("albedoMap");
	texParaNameVec.push_back("metallicMap");
	texParaNameVec.push_back("roughnessMap");
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_2D);
	if (m_loadNormal)
	{
		texIDVec.push_back(m_meshVec[0].m_TextureVec[3].texID);
		texParaNameVec.push_back("normalMap");
		texTarget.push_back(GL_TEXTURE_2D);
	}
	m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDrawPBR.world = GetMeshWorld(0);
	//World inverse matrix
	Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(0);
	//Materials
	Effect::cbDrawPBR.material = m_meshVec[0].m_PBRMaterial;
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[5]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[0].BindVertexArray();
	glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Ball::DrawPerfect(GLuint cubeTexID, int renderState, float* etas)
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	//Bind effect
	m_meshVec[0].m_pEffect->BindEffect();
	//Bind uniform
	glUniform1i(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "renderState"), renderState);
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "eta"), etas[0]);
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "etaR"), etas[1]);
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "etaG"), etas[2]);
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "etaB"), etas[3]);
	//Bind texture
	std::vector<GLuint> texIDVec;
	std::vector<const char*> texParaNameVec;
	std::vector<GLenum> texTarget;
	texIDVec.push_back(cubeTexID);
	texParaNameVec.push_back("cubeMap");
	texTarget.push_back(GL_TEXTURE_CUBE_MAP);
	m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDrawPBR.world = GetMeshWorld(0);
	//World inverse matrix
	Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(0);
	//Materials
	Effect::cbDrawPBR.material = m_meshVec[0].m_PBRMaterial;
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[5]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[0].BindVertexArray();
	glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Ball::Update(float dt)
{
	GetTransform().SetPosition(GetTransform().GetPosition() + m_direction * m_velocity * dt);
}