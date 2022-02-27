#include "Skybox.h"

void Skybox::LoadGameObject()
{
	MeshData skyboxMesh;
	skyboxMesh.SetVertexType(m_type);
    GenerateCube(skyboxMesh);
	SetCubeMap(m_skyboxName, m_textureFormat, skyboxMesh);
	m_meshVec.push_back(skyboxMesh);
}

void Skybox::Draw(int texIndex)
{
    this->GetTransform().ComputeLocalToWorldMatrix();
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Bind cube map
    m_meshVec[0].m_pEffect->BindCubeMap(m_meshVec[0].m_TextureVec[texIndex].texID);
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

void Skybox::GenerateCube(MeshData& skyboxMesh)
{
    float side = 1.0f;
    float side2 = side / 2.0f;
    //Position
    // Front
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, side2));
    // Right
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, side2));
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, side2));
    // Back
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    // Left
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    // Bottom
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(side2, -side2, side2));
    // Top
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, side2));
    skyboxMesh.m_posL.push_back(vec3(side2, side2, -side2));
    skyboxMesh.m_posL.push_back(vec3(-side2, side2, -side2));

    //Index
    GLuint index[36] = {
        0,1,2,0,2,3,
        4,5,6,4,6,7,
        8,9,10,8,10,11,
        12,13,14,12,14,15,
        16,17,18,16,18,19,
        20,21,22,20,22,23
    };
    for (int i = 0; i < 36; i++)
        skyboxMesh.m_indexVec.push_back(index[i]);
}

void Skybox::SetCubeMap(std::string skyboxName, std::string textureFormat, MeshData& skyboxMesh)
{
    const int numImages = 6;
    std::string directory = "../Project5\\Assets\\Skyboxs\\" + skyboxName;
    Texture textures[numImages];
    textures[0].name = skyboxName + "_right." + textureFormat;
    textures[1].name = skyboxName + "_left." + textureFormat;
    textures[2].name = skyboxName + "_up." + textureFormat;
    textures[3].name = skyboxName + "_down." + textureFormat;
    textures[4].name = skyboxName + "_back." + textureFormat;
    textures[5].name = skyboxName + "_front." + textureFormat;

    GLuint target[numImages] =
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    glGenTextures(1, &textures[0].texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0].texID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //stbi_set_flip_vertically_on_load(true);

    // Read texture images from files.
    for (int t = 0; t < numImages; t++)
    {
        std::string filePath = directory + "\\" + textures[t].name;
        int imgWidth, imgHeight, numComponents;

        GLubyte* imgData = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &numComponents, 0);

        if (imgData == NULL)
        {
            fprintf(stderr, "Error: Fail to read image file %s.\n", filePath.c_str());
            exit(EXIT_FAILURE);
        }
        printf("%s (%d x %d, %d components)\n", filePath.c_str(), imgWidth, imgHeight, numComponents);

        const GLint texFormat[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
        const GLenum dataFormat[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

        if (1 <= numComponents && numComponents <= 4)
        {
            glTexImage2D(target[t], 0, texFormat[numComponents - 1], imgWidth, imgHeight, 0,
                dataFormat[numComponents - 1], GL_UNSIGNED_BYTE, imgData);
        }

        else
        {
            fprintf(stderr, "Error: Unexpected image format.\n");
            exit(EXIT_FAILURE);
        }

        stbi_image_free(imgData);
        skyboxMesh.m_TextureVec.push_back(textures[t]);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void Skybox::LoadHDR(MeshData& skyboxMesh)
{
    stbi_set_flip_vertically_on_load(true);
    std::string directory = "../Project5\\Assets\\Textures\\HDR\\" + m_skyboxName;
    int width, height, nrComponents;
    float* data = stbi_loadf(directory.c_str(), &width, &height, &nrComponents, 0);
    Texture HDRMap;
    if (data)
    {
        glGenTextures(1, &HDRMap.texID);
        glBindTexture(GL_TEXTURE_2D, HDRMap.texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        skyboxMesh.m_TextureVec.push_back(HDRMap);
        printf("%s (%d x %d, %d components)\n", directory.c_str(), width, height, nrComponents);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }
}

void Skybox::GenerateEmptyCubeMap(MeshData& skyboxMesh, GLsizei width, GLsizei height, bool mip)
{
    Texture envCubeMap;
    glGenTextures(1, &envCubeMap.texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap.texID);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
            width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    if(mip)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (mip)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    skyboxMesh.m_TextureVec.push_back(envCubeMap);
}

void Skybox::LoadHDRSampler(GLsizei width, GLsizei height)
{
    MeshData skyboxMesh;
    skyboxMesh.SetVertexType(m_type);
    skyboxMesh.m_name = "SkyboxMesh";
    GenerateCube(skyboxMesh);
    LoadHDR(skyboxMesh);//texture0
    GenerateEmptyCubeMap(skyboxMesh, width, height);//texture1
    m_meshVec.push_back(skyboxMesh);
}

void Skybox::SampleHDR()
{
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Bind cube map
    m_meshVec[0].m_pEffect->BindTexture(m_meshVec[0].m_TextureVec[0].texID, "SphericalMap");

    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}