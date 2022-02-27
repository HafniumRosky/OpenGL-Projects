#include "Character.h"

/*void Character::CleanNode(Node* node)
{
	if (node->GetIndex() != -1)//Is bone node
	{
		node->SetNodeToParentTransform(mat4(1.0f));
		if (node->GetParent() != nullptr)
			node->SetFinalMatrix(node->GetParent()->GetFinalMatrix() * node->GetNodeToParentTransform());
	}
	for (int i = 0; i < node->m_childrenCount; i++)
	{
		CleanNode(node->GetChild(i));
	}
}*/

void Character::InitiAnimation()
{
	//Match node with bones
	for (int i = 0; i < m_nodeTree.size(); i++)
	{
		if (m_boneNameIndexMap.find(m_nodeTree[i].GetName()) != m_boneNameIndexMap.end())
		{
			m_nodeTree[i].SetIndex(m_boneNameIndexMap[m_nodeTree[i].GetName()]);
			m_nodeTree[i].SetOffsetMatrix(m_boneOffsetTransform[m_nodeTree[i].GetIndex()]);
			m_indexNodeMap[m_nodeTree[i].GetIndex()] = &m_nodeTree[i];
		}
	}

	//Clean Node
	//CleanNode(&m_nodeTree[0]);

	//Animation
	AnimationIK moveRightArm(vec3(-100.0f, 0.0f, 0.0f), m_nameNodeMap["Bip01 R Clavicle"], m_nameNodeMap["Bip01 R Hand"]);
	//AnimationIK moveRightArm(vec3(-100.0f, 0.0f, 0.0f), m_nameNodeMap["Bip01 R UpperArm"], m_nameNodeMap["Bip01 R Hand"]);
	GetTransform().ComputeLocalToWorldMatrix();
	moveRightArm.Initialize();
	moveRightArm.GetChainNode(1)->SetDOFMAX(vec3(radians(10.0f), radians(60.0f), radians(0.0f)));
	moveRightArm.GetChainNode(1)->SetDOFMIN(vec3(radians(-10.0f), radians(-10.0f), radians(-180.0f)));
	moveRightArm.GetChainNode(2)->SetDOFMAX(vec3(radians(10.0f), radians(90.0f), radians(10.0f)));
	moveRightArm.GetChainNode(2)->SetDOFMIN(vec3(radians(-10.0f), radians(-90.0f), radians(-75.0f)));
	moveRightArm.GetChainNode(3)->SetDOFMAX(vec3(radians(10.0f), radians(10.0f), radians(10.0f)));
	moveRightArm.GetChainNode(3)->SetDOFMIN(vec3(radians(-10.0f), radians(-10.0f), radians(-10.0f)));
	m_animationVec.push_back(moveRightArm);

	//Animation keyframe
	m_animationVec[0].m_frameTarget.push_back(vec3(-65.4f, 89.0f, 5.80001f));
	m_animationVec[0].m_frameTarget.push_back(vec3(-44.8f, 123.8f, 33.4f));
	m_animationVec[0].m_frameTarget.push_back(vec3(3.00003f, 181.0f, 21.0f));
	m_animationVec[0].m_frameTarget.push_back(vec3(3.00003f, 181.0f, 21.0f));
	m_animationVec[0].m_frameTarget.push_back(vec3(3.00003f, 181.0f, 21.0f));
	m_animationVec[0].m_frameTarget.push_back(vec3(-44.8f, 123.8f, 33.4f));
	m_animationVec[0].m_frameTarget.push_back(vec3(-65.4f, 89.0f, 5.80001f));

	m_animationVec[0].m_time.push_back(1000.0f);
	m_animationVec[0].m_time.push_back(2000.0f);
	m_animationVec[0].m_time.push_back(3000.0f);
	m_animationVec[0].m_time.push_back(4000.0f);
	m_animationVec[0].m_time.push_back(5000.0f);
	m_animationVec[0].m_time.push_back(6000.0f);

	m_animationVec[0].m_totalTime = 6000.0f;

	//Solve animation
	m_animationVec[0].SolveAnimation();
}

void Character::UpdateAnimation(float dt, vec3 target)
{
	GetTransform().ComputeLocalToWorldMatrix();
	//m_animationVec[0].SetTarget(target, GetTransform().GetWorldToLocalMatrix());
	//m_animationVec[0].IKSolveCCD();

	//Display
	m_animationVec[0].Display(dt * 1000.0f);

	mat4* finalMat = new mat4[m_indexNodeMap.size()];
	for (int i = 0; i < m_indexNodeMap.size(); i++)
	{
		finalMat[i] = m_indexNodeMap[i]->GetFinalMatrix() * m_indexNodeMap[i]->GetOffsetMatrix();
	}
	//Update uniform
	glUniformMatrix4fv(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "boneFinal"), m_indexNodeMap.size(), GL_FALSE, (const GLfloat*)finalMat);
	if (keyboard.keyState.W)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() + vec3(0.0f, 0.0f, 200.0f * dt), GetTransform().GetWorldToLocalMatrix());
	}
	if (keyboard.keyState.S)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() - vec3(0.0f, 0.0f, 200.0f * dt), GetTransform().GetWorldToLocalMatrix());
	}
	if (keyboard.keyState.A)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() + vec3(200.0f * dt, 0.0f, 0.0f), GetTransform().GetWorldToLocalMatrix());
	}
	if (keyboard.keyState.D)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() - vec3(200.0f * dt, 0.0f, 0.0f), GetTransform().GetWorldToLocalMatrix());
	}
	if (keyboard.keyState.Q)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() + vec3(0.0f, 200.0f * dt, 0.0f), GetTransform().GetWorldToLocalMatrix());
	}
	if (keyboard.keyState.E)
	{
		m_animationVec[0].SetTarget(m_animationVec[0].GetTarget() - vec3(0.0f, 200.0f * dt, 0.0f), GetTransform().GetWorldToLocalMatrix());
	}
}

void Character::LoadGameObject()
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
}

void Character::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		m_meshVec[i].m_PBRMaterial.albedo = albedo;
		m_meshVec[i].m_PBRMaterial.metallic = metallic;
		m_meshVec[i].m_PBRMaterial.roughness = roughness;
	}
}

void Character::DrawPBR()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();
		//Bind textures
		std::vector<GLuint> texIDVec;
		std::vector<const char*> texParaNameVec;
		std::vector<GLenum> texTarget;
		for (int j = 0; j < m_meshVec[i].m_TextureVec.size(); j++)
		{
			if (m_meshVec[i].m_TextureVec[j].type == "diffuse")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("albedoMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
			if (m_meshVec[i].m_TextureVec[j].type == "specular")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("metallicMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
			if (m_meshVec[i].m_TextureVec[j].type == "normal")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("normalMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
		}
		m_meshVec[i].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);


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


		m_meshVec[i].BindVertexArray();
		glDrawElements(GL_TRIANGLES, m_meshVec[i].m_indexVec.size(), GL_UNSIGNED_INT, 0);
	}
}

void Character::Update(float dt)
{
	GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.0f, 0.2f * dt, 0.0f));
}