#include "Plane.h"

void Plane::LoadGameObject(GLsizei texWidth, GLsizei texHeight)
{
	MeshData planeMesh;
	planeMesh.SetVertexType(m_type);
	GeneratePlane(planeMesh);
	GenerateTexture(planeMesh, texWidth, texHeight);
	m_meshVec.push_back(planeMesh);
}

void Plane::Draw()
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
	texIDVec.push_back(m_shadowMapID);
	texIDVec.push_back(m_environmentMapID);
	texParaNameVec.push_back("heightMap");
	texParaNameVec.push_back("normalMap");
	texParaNameVec.push_back("shadowMap");
	texParaNameVec.push_back("EnvMap");
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_CUBE_MAP);
	m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDraw.world = GetMeshWorld(0);
	//World inverse matrix
	Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(0);
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[0].BindVertexArray();
	glDrawElements(GL_PATCHES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Plane::Update(float currTime)
{
	m_meshVec[0].m_pEffect->BindEffect();
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "currTime"), currTime * 0.000002f);
}

void Plane::GeneratePlane(MeshData& planeMesh)
{
	int rowNum = m_planeVertWidth + 1;
	int colNum = m_planeHorWidth + 1;
	//Vertex properties
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			planeMesh.m_posL.push_back(vec3(-(colNum - 1) * 0.5f + j, 0.0f, -(rowNum - 1) * 0.5f + i));
			planeMesh.m_normalL.push_back(vec3(0.0f, 1.0f, 0.0f));
			planeMesh.m_tangent.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
			planeMesh.m_texcoord.push_back(vec2(1.0f / (colNum - 1) * j, 1.0f / (rowNum - 1) * i));
		}
	}
	//Indices
	for (unsigned int i = 1; i < rowNum; i++)
	{
		for (unsigned int j = 1; j < colNum; j++)
		{
			planeMesh.m_indexVec.push_back((i - 1) * rowNum + j - 1);
			planeMesh.m_indexVec.push_back((i - 1) * rowNum + j);
			planeMesh.m_indexVec.push_back(i * rowNum + j - 1);

			planeMesh.m_indexVec.push_back(i * rowNum + j - 1);
			planeMesh.m_indexVec.push_back((i - 1) * rowNum + j);
			planeMesh.m_indexVec.push_back(i * rowNum + j);
		}
	}
}

void Plane::GenerateTexture(MeshData& planeMesh, GLsizei width, GLsizei height)
{
	Texture hightMap;
	hightMap.name = "LargeWater_waves height.png";
	hightMap.type = "displace";
	Texture normalMap;
	normalMap.name = "LargeWater_perlin_wavy normals.png";
	normalMap.type = "normal";

	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\Ocean", true, &hightMap, GL_MIRRORED_REPEAT);	
	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\Ocean", true, &normalMap, GL_MIRRORED_REPEAT);

	planeMesh.m_TextureVec.push_back(hightMap);
	planeMesh.m_TextureVec.push_back(normalMap);
}

void Plane::LoadDebugObject(GLsizei texWidth, GLsizei texHeight)
{
	MeshData planeMesh;
	planeMesh.SetVertexType(m_type);
	GeneratePlane(planeMesh);
	GenerateDubugTexture(planeMesh, texWidth, texHeight);
	m_meshVec.push_back(planeMesh);
}

void Plane::GenerateDubugTexture(MeshData& planeMesh, GLsizei width, GLsizei height)
{
	Texture debugMap;
	debugMap.name = "testgrid.jpg";
	debugMap.type = "diffuse";
	Texture debugMapWithMip;
	debugMapWithMip.name = "testgrid.jpg";
	debugMapWithMip.type = "diffuse";

	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\TextureDebug", false, &debugMap, GL_MIRRORED_REPEAT);
	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\TextureDebug", true, &debugMapWithMip, GL_MIRRORED_REPEAT);

	planeMesh.m_TextureVec.push_back(debugMap);
	planeMesh.m_TextureVec.push_back(debugMapWithMip);
}

void Plane::DrawDebug()
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
	texParaNameVec.push_back("tex");
	texParaNameVec.push_back("texWithMip");
	texTarget.push_back(GL_TEXTURE_2D);
	texTarget.push_back(GL_TEXTURE_2D);
	m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
	//Set uniform
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "textureScale"), m_textureScale);

	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDraw.world = GetMeshWorld(0);
	//World inverse matrix
	Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(0);
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_meshVec[0].BindVertexArray();
	glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}