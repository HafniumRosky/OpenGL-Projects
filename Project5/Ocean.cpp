#include "Ocean.h"

void Ocean::LoadGameObject(GLsizei texWidth, GLsizei texHeight)
{
	MeshData planeMesh;
	planeMesh.SetVertexType(m_type);
	GeneratePlane(planeMesh);
	GenerateTexture(planeMesh, texWidth, texHeight);
	m_meshVec.push_back(planeMesh);
}

void Ocean::Draw()
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

void Ocean::Update(float currTime)
{
	m_meshVec[0].m_pEffect->BindEffect();
	glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "currTime"), currTime * 0.000002f);
}

void Ocean::GeneratePlane(MeshData& oceanMesh)
{
	int rowNum = m_oceanVertWidth + 1;
	int colNum = m_oceanHorWidth + 1;
	//Vertex properties
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			oceanMesh.m_posL.push_back(vec3(-(colNum - 1) * 0.5f + j, 0.0f, -(rowNum - 1) * 0.5f + i));
			oceanMesh.m_normalL.push_back(vec3(0.0f, 1.0f, 0.0f));
			oceanMesh.m_tangent.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
			oceanMesh.m_texcoord.push_back(vec2(1.0f / (colNum - 1) * j, 1.0f / (rowNum - 1) * i));
		}
	}
	//Indices
	for (unsigned int i = 1; i < rowNum; i++)
	{
		for (unsigned int j = 1; j < colNum; j++)
		{
			oceanMesh.m_indexVec.push_back((i - 1) * rowNum + j - 1);
			oceanMesh.m_indexVec.push_back((i - 1) * rowNum + j);
			oceanMesh.m_indexVec.push_back(i * rowNum + j - 1);

			oceanMesh.m_indexVec.push_back(i * rowNum + j - 1);
			oceanMesh.m_indexVec.push_back((i - 1) * rowNum + j);
			oceanMesh.m_indexVec.push_back(i * rowNum + j);
		}
	}
}

void Ocean::GenerateTexture(MeshData& oceanMesh, GLsizei width, GLsizei height)
{
	Texture hightMap;
	hightMap.name = "LargeWater_waves height.png";
	hightMap.type = "displace";
	Texture normalMap;
	normalMap.name = "LargeWater_perlin_wavy normals.png";
	normalMap.type = "normal";

	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\Ocean", true, &hightMap, GL_MIRRORED_REPEAT);	
	SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\Ocean", true, &normalMap, GL_MIRRORED_REPEAT);

	oceanMesh.m_TextureVec.push_back(hightMap);
	oceanMesh.m_TextureVec.push_back(normalMap);
}