#include "GameObject.h"

void GameObject::LoadGameObjectFromFile(std::string path, std::string textureDir, int existingMeshNum)
{
	Assimp::Importer importer;
	const aiScene* pAIScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!pAIScene
		|| pAIScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE
		|| !pAIScene->mRootNode)
	{
		printf("Error parsing '%s': '%s'\n", path.c_str(), importer.GetErrorString());
		return;
	}
	//Resize the vectors of the properties
	m_existMeshNum = existingMeshNum;
	this->m_meshVec.resize(pAIScene->mNumMeshes + existingMeshNum);
	for (int i = existingMeshNum; i < m_meshVec.size(); i++)
		m_meshVec[i].SetVertexType(m_type, m_hasBone);

	//Get the number of node
	m_nodeNum = 1;
	GetNodeNum(pAIScene->mRootNode);
	m_nodeTree.resize(m_nodeNum);

	//Set root node
	Node rootNode;
	rootNode.SetIndex(-2);
	rootNode.SetNodeToParentTransform(pAIScene->mRootNode->mTransformation.Inverse());
	//rootNode.GetFinalTransform().SetPosition(rootNode.GetNodeToParentTransform() * vec4(vec3(0.0f), 1.0f));
	//rootNode.GetFinalTransform().m_quat = quat_cast(rootNode.GetNodeToParentTransform());
	//rootNode.SetFinalMatrix(rootNode.GetNodeToParentTransform() * glm::translate(mat4(1), rootNode.GetFinalTransform().GetPosition()) * mat4_cast(rootNode.GetFinalTransform().m_quat));
	rootNode.SetFinalMatrix(rootNode.GetNodeToParentTransform());
	std::string nodeName = pAIScene->mRootNode->mName.data;
	rootNode.SetName(nodeName);
	m_nodeTree[0] = rootNode;
	m_rootNode = &m_nodeTree[0];
	m_nameNodeMap.insert(make_pair(nodeName, &m_nodeTree[0]));
	//m_nameNodeMap[nodeName] = &m_nodeTree[0];
	m_nodeCount = 1;
	LoadMeshFromScene(pAIScene->mRootNode, pAIScene, textureDir, &m_nodeTree[0]);
}

void GameObject::GetNodeNum(const aiNode* pCurrentNode)
{
	int num = pCurrentNode->mNumChildren;
	m_nodeNum += num;
	for (unsigned int i = 0; i < pCurrentNode->mNumChildren; i++)
		GetNodeNum(pCurrentNode->mChildren[i]);
}

void GameObject::LoadMeshFromScene(const aiNode* pCurrentNode, const aiScene* pAIScene, std::string textureDir, Node* node)
{
	//load meshes in the current node
	for (unsigned int i = 0; i < pCurrentNode->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pAIScene->mMeshes[pCurrentNode->mMeshes[i]];
		this->LoadMeshData(pMesh, pCurrentNode->mMeshes[i], pAIScene, textureDir);
	}

	node->ResizeChildren(pCurrentNode->mNumChildren);
	//Load meshes in children nodes
	for (unsigned int i = 0; i < pCurrentNode->mNumChildren; i++)
	{
		Node childNode;
		unsigned int childIndex = m_nodeCount;
		childNode.SetIndex(-1);
		childNode.SetNodeToParentTransform(pCurrentNode->mChildren[i]->mTransformation);
		childNode.SetParent(node);
		//childNode.GetFinalTransform().SetPosition(glm::translate(mat4(1), node->GetFinalTransform().GetPosition()) * mat4_cast(node->GetFinalTransform().GetQuaternion()) * childNode.GetNodeToParentTransform() * vec4(vec3(0.0f), 1.0f));
		//childNode.GetFinalTransform().m_quat = node->GetFinalTransform().m_quat * childNode.GetSelfTransform().m_quat;
		childNode.SetFinalMatrix(node->GetFinalMatrix() * childNode.GetNodeToParentTransform());
		//childNode.SetFinalMatrix(glm::translate(mat4(1), childNode.GetFinalTransform().GetPosition()) * mat4_cast(childNode.GetFinalTransform().m_quat));
		std::string nodeName = pCurrentNode->mChildren[i]->mName.data;
		childNode.SetName(nodeName);
		m_nodeTree[m_nodeCount] = childNode;
		m_nodeCount++;
		m_nameNodeMap.insert(make_pair(nodeName, &m_nodeTree[childIndex]));
		//m_nameNodeMap[nodeName] = &m_nodeTree[childIndex];
		node->AddChild(&m_nodeTree[childIndex]);
		node->m_childrenCount++;
		this->LoadMeshFromScene(pCurrentNode->mChildren[i], pAIScene, textureDir, &m_nodeTree[childIndex]);
	}
}


void GameObject::LoadMeshData(const aiMesh* pMesh, unsigned int index, const aiScene* pAIScene, std::string textureDir)
{
	//In case we don't have the matching data
	const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);
	const aiColor4D black(0.0f, 0.0f, 0.0f, 0.0f);
	unsigned int meshIndex;
	if (m_existMeshNum == 0)
		meshIndex = index;
	else
		meshIndex = m_existMeshNum;
	this->m_meshVec[meshIndex].m_name = pMesh->mName.C_Str();
	//Load vertex data
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = pMesh->HasPositions() ? &(pMesh->mVertices[i]) : &zeroVec;
		const aiVector3D* pNormal = pMesh->HasNormals() ? &(pMesh->mNormals[i]) : &zeroVec;
		const aiVector3D* pTangent = pMesh->HasTangentsAndBitangents() ? &(pMesh->mTangents[i]) : &zeroVec;
		//const aiVector3D* pBiTangent = pMesh->HasTangentsAndBitangents() ? &(pMesh->mBitangents[i]) : &zeroVec;
		const aiColor4D* pColor = pMesh->HasVertexColors(0) ? &(pMesh->mColors[0][i]) : &black;
		const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &zeroVec;

		//Load model position
		this->m_meshVec[meshIndex].m_posL.push_back(vec3(pPos->x, pPos->y, pPos->z));
		//Load model normal
		if(this->m_meshVec[meshIndex].m_dataState.normal)
			this->m_meshVec[meshIndex].m_normalL.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
		if(this->m_meshVec[meshIndex].m_dataState.tangent)
			this->m_meshVec[meshIndex].m_tangent.push_back(vec4(pTangent->x, pTangent->y, pTangent->z, 1.0f));
		if (this->m_meshVec[meshIndex].m_dataState.color)
			this->m_meshVec[meshIndex].m_color.push_back(vec4(pColor->r, pColor->g, pColor->b, pColor->a));
		if (this->m_meshVec[meshIndex].m_dataState.tex)
			this->m_meshVec[meshIndex].m_texcoord.push_back(vec2(pTexCoord->x, pTexCoord->y));

		//Prepare bone data
		if (pMesh->HasBones())
		{
			BoneData boneData;
			this->m_meshVec[meshIndex].m_boneDataVec.push_back(boneData);
			this->m_meshVec[meshIndex].m_boneNum.push_back(0);
		}
	}

	//Load index data
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		aiFace face = pMesh->mFaces[i];
		if (face.mNumIndices < 3)
			continue;
		assert(face.mNumIndices == 3);
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			this->m_meshVec[meshIndex].m_indexVec.push_back(face.mIndices[j]);
		}
	}
	//Accumulate the number of vertices
	this->m_verticesCount += pMesh->mNumVertices;

	//Load material
	if (pMesh->mMaterialIndex >= 0)
	{
		aiMaterial* pMaterial = pAIScene->mMaterials[pMesh->mMaterialIndex];
		aiColor3D color;
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		m_meshVec[meshIndex].m_material.ambient = vec4(color.r, color.g, color.g, 1.0f);
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m_meshVec[meshIndex].m_material.diffuse = vec4(color.r, color.g, color.g, 1.0f);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		m_meshVec[meshIndex].m_material.specular = vec4(color.r, color.g, color.g, 1.0f);
		float gloss;
		pMaterial->Get(AI_MATKEY_SHININESS, gloss);
		m_meshVec[meshIndex].m_material.gloss = gloss;
		//May add more types
		LoadMeshTexture(pMaterial, aiTextureType_DIFFUSE, "diffuse", index, textureDir);
		LoadMeshTexture(pMaterial, aiTextureType_NORMALS, "normal", index, textureDir);
		LoadMeshTexture(pMaterial, aiTextureType_SPECULAR, "specular", index, textureDir);
	}

	//Load bones
	if (pMesh->HasBones())
	{
		for (int i = 0; i < pMesh->mNumBones; i++)
		{
			//Add additional bones
			unsigned int boneIndex = 0;
			std::string boneName(pMesh->mBones[i]->mName.data);
			//std::cout << boneName << std::endl;
			if (m_boneNameIndexMap.find(boneName) == m_boneNameIndexMap.end())
			{
				boneIndex = m_boneOffsetTransform.size();
				mat4 offsetTransform;
				m_boneOffsetTransform.push_back(offsetTransform);
				m_boneNameIndexMap[boneName] = boneIndex;
			}
			else
				boneIndex = m_boneNameIndexMap[boneName];

			//Load bone offset matrix
			aiMatrix4x4 offsetMat = pMesh->mBones[i]->mOffsetMatrix;
			m_boneOffsetTransform[boneIndex] =
				mat4(offsetMat.a1, offsetMat.b1, offsetMat.c1, offsetMat.d1,
					offsetMat.a2, offsetMat.b2, offsetMat.c2, offsetMat.d2,
					offsetMat.a3, offsetMat.b3, offsetMat.c3, offsetMat.d3,
					offsetMat.a4, offsetMat.b4, offsetMat.c4, offsetMat.d4);

			//Load the bone into vertex data
			for (int j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
			{
				int vertexIndex = pMesh->mBones[i]->mWeights[j].mVertexId;
				float weight = pMesh->mBones[i]->mWeights[j].mWeight;
				m_meshVec[meshIndex].m_boneDataVec[vertexIndex].boneIndex[m_meshVec[meshIndex].m_boneNum[vertexIndex]] = boneIndex;
				m_meshVec[meshIndex].m_boneDataVec[vertexIndex].boneWeight[m_meshVec[meshIndex].m_boneNum[vertexIndex]] = weight;
				m_meshVec[meshIndex].m_boneNum[vertexIndex] += 1;
			}
		}
	}
}

void GameObject::LoadMeshTexture(aiMaterial* mat, aiTextureType type, std::string typeName, unsigned int index, std::string textureDir)
{
	Texture texture;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString textureName;
		mat->GetTexture(type, i, &textureName);
		std::string name = textureName.C_Str();
		int namePos = name.find_last_of('\\');
		name = (name.substr(namePos + 1));
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexture.size(); j++)
		{
			if (loadedTexture[j].name.data() == name)
			{
				m_meshVec[index].m_TextureVec.push_back(loadedTexture[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			texture.type = typeName;
			texture.name = name;
			SetUpTextureMapFromFile(textureDir, true, &texture);
			m_meshVec[index].m_TextureVec.push_back(texture);
			loadedTexture.push_back(texture);
		}
	}
}

void GameObject::SetUpTextureMapFromFile(const std::string directory, bool mipmap, Texture* pTexture, GLint texClamp, int* pNumColorChannels)
{
	//Complete path
	std::string completePath = directory + "\\" + pTexture->name;

	//stbi_set_flip_vertically_on_load(true);

	// Read texture image from file.

	int imgWidth, imgHeight, numComponents;

	GLubyte* imgData = stbi_load(completePath.c_str(), &imgWidth, &imgHeight, &numComponents, 0);

	if (imgData == NULL)
	{
		fprintf(stderr, "Error: Fail to read image file %s.\n", completePath.c_str());
	}
	printf("%s (%d x %d, %d components)\n", completePath.c_str(), imgWidth, imgHeight, numComponents);

	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texClamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texClamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mipmap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const GLint texFormat[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
	const GLenum dataFormat[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

	if (1 <= numComponents && numComponents <= 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, texFormat[numComponents - 1], imgWidth, imgHeight, 0,
			dataFormat[numComponents - 1], GL_UNSIGNED_BYTE, imgData);
	}
	else
	{
		fprintf(stderr, "Error: Unexpected image format.\n");
		exit(EXIT_FAILURE);
	}

	stbi_image_free(imgData);

	if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);

	// Return these values.
	pTexture->texID = tid;
	if (pNumColorChannels != NULL) *pNumColorChannels = numComponents;
}

void GameObject::InputAssemble()
{
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		m_meshVec[i].BindVertexArray();
		m_meshVec[i].SetVertexBuffer();
		m_meshVec[i].SetIndexBuffer();
		m_meshVec[i].BindBuffers();
	}
}

void GameObject::InputAssemble(int index)
{
	m_meshVec[index].BindVertexArray();
	m_meshVec[index].SetVertexBuffer();
	m_meshVec[index].SetIndexBuffer();
	m_meshVec[index].BindBuffers();
}

int GameObject::PickVertex(int meshIndex, vec2 pickPos, mat4 proj, mat4 view, std::vector<unsigned int>& indeices, vec3 viewDir)
{
	int shortestIndex = 0;;
	m_transform.ComputeLocalToWorldMatrix();
	vec4 clipPos = proj * view * m_transform.GetLoaclToWorldMatrix() * vec4(m_meshVec[meshIndex].m_posL[0], 1.0f);
	vec2 screenPos = vec2(clipPos.x / clipPos.w, clipPos.y / clipPos.w);
	float shortestDistance = glm::distance(pickPos, screenPos);
	for (int i = 1; i < m_meshVec[meshIndex].m_posL.size(); i++)
	{
		clipPos = proj * view * m_transform.GetLoaclToWorldMatrix() * vec4(m_meshVec[meshIndex].m_posL[i], 1.0f);
		screenPos = vec2(clipPos.x / clipPos.w, clipPos.y / clipPos.w);
		float currDistance = glm::distance(pickPos, screenPos);
		if (currDistance < shortestDistance)
		{
			if (!m_meshVec[meshIndex].m_dataState.normal)
			{
				shortestIndex = i;
				shortestDistance = currDistance;
			}
			else
			{
				if (glm::dot(viewDir, vec3(glm::inverse(glm::transpose(m_transform.GetLoaclToWorldMatrix())) * vec4(m_meshVec[meshIndex].m_normalL[i], 0.0f))) <= 0)
				{
					shortestIndex = i;
					shortestDistance = currDistance;
				}
			}
		}
	}
	//Get all vertex that have same position
	indeices.clear();
	indeices.resize(0);
	for (int i = 0; i < m_meshVec[meshIndex].m_posL.size(); i++)
	{
		if (m_meshVec[meshIndex].m_posL[i] == m_meshVec[meshIndex].m_posL[shortestIndex])
		{
			indeices.push_back(i);
		}
	}
	if (shortestDistance < 0.02f)
	{
		return shortestIndex;
	}
	else
		return -1;
}