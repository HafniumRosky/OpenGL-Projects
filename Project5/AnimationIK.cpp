#include "AnimationIK.h"
#include <iostream>

void AnimationIK::BuildIKChain()
{
	Node* curr = m_effector;
	while (curr != m_IKChainStart)
	{
		m_IKChain.push_back(curr);
		curr = curr->GetParent();
	}
	m_IKChain.push_back(curr);
}

void AnimationIK::Initialize()
{
	BuildIKChain();
}

void AnimationIK::UpdateIKChain(int index)
{
	//Update from current node to effector
	for (int i = index; i >= 0; i--)
	{
		m_IKChain[i]->UpdateSelf();
	}
}

void AnimationIK::IKSolveCCD()
{
	for (int i = 0; i < m_IKIterationNum; i++)
	{
		for (int j = 1; j < m_IKChain.size(); j++)
		{
			//Get to bone space
			vec3 effectorR = m_IKChain[0]->GetFinalMatrix() * vec4(vec3(0.0f), 1.0);
			vec3 effectorB = glm::inverse(m_IKChain[j]->GetFinalMatrix()) * vec4(effectorR, 1.0f);
			//vec3 effectorB = currEffectorMat * vec4(vec3(0.0f), 1.0f);
			vec3 targetB = glm::inverse(m_IKChain[j]->GetFinalMatrix()) * vec4(m_target, 1.0f);
			//Check distance
			if (glm::distance(effectorB, targetB) < 1e-3)
			{
				i = m_IKIterationNum;
				break;
			}
			//Ckeck angle
			vec3 effectorBVec = glm::normalize(effectorB);
			vec3 targetBVec = glm::normalize(targetB);
			float TdotE = glm::dot(targetBVec, effectorBVec);
			if (TdotE > 1 - 1e-6)
			{
				continue;
			}
			float angle = glm::acos(TdotE);
			angle = glm::clamp(angle, -m_angleLimit, m_angleLimit);
			//Rotate
			vec3 axisB = glm::normalize(glm::cross(effectorBVec, targetBVec));
			m_IKChain[j]->GetSelfTransform().m_quat *= glm::angleAxis(angle, axisB);
			//Check DOF
			vec3 eular = glm::eulerAngles(m_IKChain[j]->GetSelfTransform().m_quat);
			eular = glm::clamp(eular, m_IKChain[j]->GetDOFMIN(), m_IKChain[j]->GetDOFMAX());
			m_IKChain[j]->GetSelfTransform().m_quat = quat(eular);
			UpdateIKChain(j);
		}
	}
	m_IKChain[m_IKChain.size() - 1]->UpdateSelfAndChildren();
}

void AnimationIK::SolveAnimation()
{
	for (int i = 0; i < m_frameTarget.size(); i++)
	{
		m_target = m_frameTarget[i];
		IKSolveCCD();
		KeyFrame key;
		for (int j = 0; j < m_IKChain.size(); j++)
		{
			key.m_selfQuat.push_back(m_IKChain[j]->GetSelfTransform().m_quat);
		}
		m_keyFrame.push_back(key);
	}
}

void AnimationIK::Display(float dt)
{
	int currIndex = 0;
	for (int i = 0; i < m_time.size(); i++)
	{
		if (i != m_time.size() - 1)
		{
			if (m_time[i] <= m_timer && m_time[i + 1] > m_timer)
			{
				currIndex = i;
				break;
			}
		}
		else
		{
			if (m_time[i] <= m_timer)
			{
				currIndex = i;
				break;
			}
		}
	}
	float interpolation = (m_timer - m_time[currIndex]) / 1000.0f;
	//Update
	for (int j = m_IKChain.size() - 1; j >= 0; j--)
	{
		m_IKChain[j]->GetSelfTransform().m_quat = m_keyFrame[currIndex].m_selfQuat[j] * (1 - interpolation) + interpolation * m_keyFrame[currIndex + 1].m_selfQuat[j];
		m_IKChain[j]->SetFinalMatrix(m_IKChain[j]->GetParent()->GetFinalMatrix() * m_IKChain[j]->GetNodeToParentTransform() * mat4_cast(m_IKChain[j]->GetSelfTransform().m_quat));
		for (int k = 0; k < m_IKChain[j]->m_childrenCount; k++)
		{
			m_IKChain[j]->GetChild(k)->UpdateSelfAndChildren();
		}
	}
	if (m_timer >= m_totalTime)
		m_timer = 0.0f;
	m_timer += dt;
}