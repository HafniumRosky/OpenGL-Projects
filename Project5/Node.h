#ifndef NODE_H
#define NODE_H

#include <glm.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <vector>
#include <map>
#include <string>
#include "Transform.h"

class Node
{
private:
	int m_index;
	Node* m_parent = nullptr;
	std::vector<Node*> m_children;

	mat4 m_nodeToParentTransform = mat4(1.0f);		//Transform to parent node coordinate space
	//mat4 m_transform = mat4(1.0f);		//Self transformation
	mat4 m_finalMatrix = mat4(1.0f);	//Transformation including parent self transformation

	Transform m_selfTransform;			//Transformation that cannot be affected by parent nodes in bone space
	Transform m_finalTransfrom;			//Transformation that can be affected by parent nodes in bone space (must keep all the parents up-to-date before use)

	vec3 m_DOFMAX = vec3(glm::radians(180.0f));
	vec3 m_DOFMIN = vec3(glm::radians(-180.0f));

	//Bone
	std::string m_name;
	mat4 m_offsetMatrix = mat4(1.0f);				//Transform local to bone space

public:
	int m_childrenCount = 0;

	void SetIndex(int index)
	{
		m_index = index;
	}

	int GetIndex()
	{
		return m_index;
	}

	void SetParent(Node* node)
	{
		m_parent = node;
	}

	Node* GetParent()
	{
		return m_parent;
	}

	void ResizeChildren(int num)
	{
		m_children.resize(num);
	}

	void AddChild(Node* node)
	{
		m_children[m_childrenCount] = node;
	}

	Node* GetChild(GLuint index)
	{
		if (index < m_children.size())
			return m_children[index];
		else
			return nullptr;
	}

	void SetNodeToParentTransform(aiMatrix4x4 transformMat)
	{
		m_nodeToParentTransform = mat4(transformMat.a1, transformMat.b1, transformMat.c1, transformMat.d1,
			transformMat.a2, transformMat.b2, transformMat.c2, transformMat.d2,
			transformMat.a3, transformMat.b3, transformMat.c3, transformMat.d3,
			transformMat.a4, transformMat.b4, transformMat.c4, transformMat.d4);
	}

	void SetNodeToParentTransform(mat4 mat)
	{
		m_nodeToParentTransform = mat;
	}

	mat4 GetNodeToParentTransform()
	{
		return m_nodeToParentTransform;
	}

	Transform& GetSelfTransform()
	{
		return m_selfTransform;
	}

	Transform& GetFinalTransform()
	{
		return m_finalTransfrom;
	}

	void SetOffsetMatrix(mat4 offsetMatrix)
	{
		m_offsetMatrix = offsetMatrix;
	}

	mat4 GetOffsetMatrix()
	{
		return m_offsetMatrix;
	}

	void SetName(std::string name)
	{
		m_name = name;
	}

	std::string GetName()
	{
		return m_name;
	}

	mat4 GetFinalMatrix()
	{
		return m_finalMatrix;
	}

	void SetFinalMatrix(mat4 mat)
	{
		m_finalMatrix = mat;
	}

	void SetDOFMAX(vec3 dof)
	{
		m_DOFMAX = dof;
	}

	vec3 GetDOFMAX()
	{
		return m_DOFMAX;
	}


	void SetDOFMIN(vec3 dof)
	{
		m_DOFMIN = dof;
	}

	vec3 GetDOFMIN()
	{
		return m_DOFMIN;
	}
	void UpdateSelf();
	void UpdateSelfAndChildren();
};


#endif