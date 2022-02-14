#include "GameObject.h"

void GameObject::LoadGameObjectFromFile(std::string path, std::string textureDir)
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
	this->m_meshVec.resize(pAIScene->mNumMeshes);
	for (int i = 0; i < m_meshVec.size(); i++)
		m_meshVec[i].SetVertexType(m_type);
	LoadMeshFromScene(pAIScene->mRootNode, pAIScene, textureDir);
}

void GameObject::LoadMeshFromScene(const aiNode* pCurrentNode, const aiScene* pAIScene, std::string textureDir)
{
	//load meshes in the current node
	for (unsigned int i = 0; i < pCurrentNode->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pAIScene->mMeshes[pCurrentNode->mMeshes[i]];
		this->LoadMeshData(pMesh, pCurrentNode->mMeshes[i], pAIScene, textureDir);
	}

	//Load meshes in children nodes
	for (unsigned int i = 0; i < pCurrentNode->mNumChildren; i++)
	{
		this->LoadMeshFromScene(pCurrentNode->mChildren[i], pAIScene, textureDir);
	}
}


void GameObject::LoadMeshData(const aiMesh* pMesh, unsigned int index, const aiScene* pAIScene, std::string textureDir)
{
	//In case we don't have the matching data
	const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);
	const aiColor4D black(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_meshVec[index].m_name = pMesh->mName.C_Str();
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
		this->m_meshVec[index].m_posL.push_back(vec3(pPos->x, pPos->y, pPos->z));
		//Load model normal
		if(this->m_meshVec[index].m_dataState.normal)
			this->m_meshVec[index].m_normalL.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
		if(this->m_meshVec[index].m_dataState.tangent)
			this->m_meshVec[index].m_tangent.push_back(vec4(pTangent->x, pTangent->y, pTangent->z, 1.0f));
		if (this->m_meshVec[index].m_dataState.color)
			this->m_meshVec[index].m_color.push_back(vec4(pColor->r, pColor->g, pColor->b, pColor->a));
		if (this->m_meshVec[index].m_dataState.tex)
			this->m_meshVec[index].m_texcoord.push_back(vec2(pTexCoord->x, pTexCoord->y));
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
			this->m_meshVec[index].m_indexVec.push_back(face.mIndices[j]);
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
		m_meshVec[index].m_material.ambient = vec4(color.r, color.g, color.g, 1.0f);
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m_meshVec[index].m_material.diffuse = vec4(color.r, color.g, color.g, 1.0f);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		m_meshVec[index].m_material.specular = vec4(color.r, color.g, color.g, 1.0f);
		float gloss;
		pMaterial->Get(AI_MATKEY_SHININESS, gloss);
		m_meshVec[index].m_material.gloss = gloss;
		//May add more types
		LoadMeshTexture(pMaterial, aiTextureType_DIFFUSE, "diffuse", index, textureDir);
		LoadMeshTexture(pMaterial, aiTextureType_NORMALS, "normal", index, textureDir);
		LoadMeshTexture(pMaterial, aiTextureType_SPECULAR, "specular", index, textureDir);
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