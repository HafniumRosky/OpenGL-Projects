#ifndef ANIMATION_IK_H
#define ANIMATION_IK_H

#include <string>
#include <vector>
#include "Node.h"

struct KeyFrame
{
	std::vector<glm::quat> m_selfQuat;
};

class AnimationIK
{
private:
	std::string m_animationName;
	Node* m_IKChainStart = nullptr;
	Node* m_effector = nullptr;
	vec3 m_target;

	float m_timer = 0.0f;
	
	//IK
	//IK Chain
	std::vector<Node*> m_IKChain;
	//IK iteration
	unsigned int m_IKIterationNum = 6;
	//IK rotation limit
	float m_angleLimit = glm::radians(30.0f);



	void BuildIKChain();
	void UpdateIKChain(int index);

public:
	float m_totalTime = 4000.0f;
	std::vector<KeyFrame> m_keyFrame;
	std::vector<vec3> m_frameTarget;
	std::vector<float> m_time;
	AnimationIK(vec3 target, Node* start, Node* effector)
	{
		m_target = target;
		m_IKChainStart = start;
		m_effector = effector;
	}
	void Initialize();
	void IKSolveCCD();
	void SolveAnimation();
	void Display(float dt);
	void SetTarget(vec3 target, mat4 inverseWorldMatrix)
	{
		m_target = target;
	}

	vec3 GetTarget()
	{
		return m_target;
	}

	Node* GetEffector()
	{
		return m_effector;
	}

	Node* GetChainNode(int index)
	{
		return m_IKChain[index];
	}
};

#endif