#include "Node.h"
#include <iostream>

void Node::UpdateSelf()
{
	if (m_parent != nullptr)
	{
		m_finalMatrix = m_parent->m_finalMatrix * m_nodeToParentTransform * mat4_cast(m_selfTransform.m_quat);
	}
}

void Node::UpdateSelfAndChildren()
{
	//Update self
	UpdateSelf();
	//Update children
	if (!m_children.empty())
	{
		for (int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->UpdateSelfAndChildren();
		}
	}
}