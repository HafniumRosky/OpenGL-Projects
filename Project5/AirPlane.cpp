#include "AirPlane.h"

void AirPlane::LoadGameObject()
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		if (m_meshVec[i].m_name == "Cube.004")
		{
			m_meshVec[i].m_childTransform.SetPosition(vec3(135.0f, 75.0f, 0.0f));
		}
		if (m_meshVec[i].m_name == "Cube.021")
		{
			m_meshVec[i].m_childTransform.SetPosition(vec3(-232.0f, 40.0f, 0.0f));
		}
		if (m_meshVec[i].m_name == "Cube.022")
		{
			m_meshVec[i].m_childTransform.SetPosition(vec3(0.0f, 20.0f, 57.5f));
		}
		if (m_meshVec[i].m_name == "Cube.023")
		{
			m_meshVec[i].m_childTransform.SetPosition(vec3(0.0f, 20.0f, -57.5f));
		}
	}
}

void AirPlane::Draw()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();
		//Bind textures
		for (int j = 0; j < m_meshVec[i].m_TextureVec.size(); j++)
		{
			std::vector<GLuint> texIDVec;
			std::vector<const char*> texParaNameVec;
			std::vector<GLenum> texTarget;
			if (m_meshVec[i].m_TextureVec[j].type == "diffuse")
			{
				//m_meshVec[i].m_pEffect->BindTexture(m_meshVec[i].m_TextureVec[j].texID, "texture1");
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("texture1");
				texTarget.push_back(GL_TEXTURE_2D);
				m_meshVec[i].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
			}
		}

		//Update Constant Buffer(change every draw)
		//World matrix
		Effect::cbDraw.world = GetMeshWorld(i);
		//World inverse matrix
		Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(i);
		//Material
		Effect::cbDraw.material = m_meshVec[i].m_material;
		glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[i].m_pEffect->m_UBOid[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		m_meshVec[i].BindVertexArray();
		glDrawElements(GL_TRIANGLES, m_meshVec[i].m_indexVec.size(), GL_UNSIGNED_INT, 0);
	}
}

void AirPlane::Fly(float dt)
{
	//Pitch
	if (keyboard.keyState.W)
	{
		GetTransform().Rotate(vec3(0.0f, 0.0f, -2.0f * dt));
		m_isDiving = true;
	}
	else
		m_isDiving = false;
	if (!m_isDiving)
	{
		if (keyboard.keyState.S)
			GetTransform().Rotate(vec3(0.0f, 0.0f, 2.0f * dt));
	}

	//Roll
	if (keyboard.keyState.D)
	{
		GetTransform().Rotate(vec3(2.0f * dt, 0.0f, 0.0f));
		m_isRollingCW = true;
	}
	else
		m_isRollingCW = false;
	if (!m_isRollingCW)
	{
		if (keyboard.keyState.A)
		{
			GetTransform().Rotate(vec3(-2.0f * dt, 0.0f, 0.0f));
		}
	}

	//Yaw
	if (keyboard.keyState.E)
	{
		GetTransform().Rotate(vec3(0.0f, -0.5f * dt, 0.0f));
		m_isTruningRight = true;
	}
	else
		m_isTruningRight = false;
	if (!m_isTruningRight)
	{
		if (keyboard.keyState.Q)
		{
			GetTransform().Rotate(vec3(0.0f, 0.5f * dt, 0.0f));
		}
	}

	//Change rotation method
	if (keyboard.keyState.N && m_stateChangeTimer > 1.0f)
	{
		m_stateChangeTimer = 0.0f;
		if (GetTransform().UsingQuaternion())
			GetTransform().UseEularAngleRotation();
		else
			GetTransform().UseQuaternionRotation();
	}
	m_stateChangeTimer += dt;
}

void AirPlane::RollPropeller(float dt)
{
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		if (m_meshVec[i].m_name == "Cube.004")
		{
			m_meshVec[i].m_childTransform.SetRotation(m_meshVec[i].m_childTransform.GetRotation() + vec3(20.0f * dt, 0.0f, 0.0f));
		}
	}
}

void AirPlane::Update(float dt)
{
	Fly(dt);
	RollPropeller(dt);
}