#include "LightProbe.h"

void LightProbe::GeneratePlane(MeshData& texturePlane)
{
    float side = 1.0f;
    float side2 = side / 2.0f;
    //Position
    texturePlane.m_posL.push_back(vec3(side2, side2, side2));
    texturePlane.m_posL.push_back(vec3(side2, -side2, side2));
    texturePlane.m_posL.push_back(vec3(-side2, -side2, side2));
    texturePlane.m_posL.push_back(vec3(-side2, side2, side2));
    //Texture Coordinates
    //texturePlane.m_texcoord.push_back(vec2(1.0f, 1.0f));
    //texturePlane.m_texcoord.push_back(vec2(1.0f, 0.0f));
    //texturePlane.m_texcoord.push_back(vec2(0.0f, 0.0f));
    //texturePlane.m_texcoord.push_back(vec2(0.0f, 1.0f));
    //Index
    GLuint index[6] = {0,1,2,0,2,3};
    for (int i = 0; i < 6; i++)
        texturePlane.m_indexVec.push_back(index[i]);
}

void LightProbe::GenerateDualParaMap(GLsizei& width, GLsizei& height, MeshData* texturePlane)
{
    int imgWidth, imgHeight, numComponents;

    Texture frontTex;
    Texture backTex;
    glGenTextures(1, &frontTex.texID);
    glGenTextures(1, &backTex.texID);

    glBindTexture(GL_TEXTURE_2D, frontTex.texID);

    if (texturePlane != nullptr)
    {
        m_hasDualParaMap = false;
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texturePlane->m_TextureVec.push_back(frontTex);
    }
    else
    {
        std::string path = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_lightPorbeName + "\\DualParaboloidFront.hdr";
        GLfloat* imgDataF = stbi_loadf(path.c_str(), &imgWidth, &imgHeight, &numComponents, 0);
        m_hasDualParaMap = true;
        width = imgWidth;
        height = imgHeight;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, imgWidth, imgHeight, 0, GL_RGB, GL_FLOAT, imgDataF);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        m_dualParaMap[0] = frontTex;
        stbi_image_free(imgDataF);
    }

    glBindTexture(GL_TEXTURE_2D, backTex.texID);

    if (texturePlane != nullptr)
    {
        m_hasDualParaMap = false;
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texturePlane->m_TextureVec.push_back(backTex);
    }
    else
    {
        std::string path = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_lightPorbeName + "\\DualParaboloidBack.hdr";
        GLfloat* imgDataB = stbi_loadf(path.c_str(), &imgWidth, &imgHeight, &numComponents, 0);
        m_hasDualParaMap = true;
        width = imgWidth;
        height = imgHeight;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, imgWidth, imgHeight, 0, GL_RGB, GL_FLOAT, imgDataB);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        m_dualParaMap[0] = backTex;
        stbi_image_free(imgDataB);
    }
}

void LightProbe::GenerateLUT(GLsizei& size, MeshData* texturePlane)
{
    Texture LUTTexture;
    glGenTextures(1, &LUTTexture.texID);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, LUTTexture.texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, size, size, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texturePlane != nullptr)
        texturePlane->m_TextureVec.push_back(LUTTexture);
}

void LightProbe::LoadLightProbe(GLsizei irrWidth, GLsizei irrHeight, GLsizei prefilterSize, GLsizei LUTSize)
{
    /*//Dual-paraboloid map
    //Check if the map is saved
    std::string MapPath = "../Project5\\Assets\\Precomputation\\Probe" + m_lightPorbeName + "\\DualParaboloidFront.hdr";
    FILE* f = std::fopen(MapPath.c_str(), "rb");
    if (f == NULL)
    {
        //Create map
        MeshData texturePlane;
        texturePlane.SetVertexType(m_type);
        texturePlane.m_name = "TexturePlane";
        GeneratePlane(texturePlane);
        GenerateDualParaMap(width, height, &texturePlane);
        texturePlane.m_TextureVec.push_back(m_meshVec[0].m_TextureVec[1]);
        m_meshVec.push_back(texturePlane);
    }
    else
    {
        GenerateDualParaMap(width, height);
    }*/
    
    //irradiance map
    GenerateEmptyCubeMap(m_meshVec[0], irrWidth, irrHeight); //texture0, 2(Global)

    //Prifilter map
    GenerateEmptyCubeMap(m_meshVec[0], prefilterSize, prefilterSize, true); //texture0, 3(Global)

    //BRDF LUT
    MeshData texturePlane;//mesh 1
    texturePlane.SetVertexType(m_type);
    texturePlane.m_name = "TexturePlane";
    GeneratePlane(texturePlane);
    GenerateLUT(LUTSize, &texturePlane);//texture 1, 0
    m_meshVec.push_back(texturePlane);
}

void LightProbe::LoadLocalLightProbe(GLsizei irrWidth, GLsizei irrHeight, GLsizei prefilterSize, GLsizei LUTSize)
{
    //Sampler
    MeshData textureCube;
    textureCube.SetVertexType(m_type);
    textureCube.m_name = "TextureCube";
    GenerateCube(textureCube);
    GenerateEmptyCubeMap(textureCube, irrWidth, irrHeight);//texture0, 0(Local)
    m_meshVec.push_back(textureCube);
    //irradiance map
    GenerateEmptyCubeMap(m_meshVec[0], irrWidth, irrHeight); //texture0, 1(Local)

    //Prifilter map
    GenerateEmptyCubeMap(m_meshVec[0], prefilterSize, prefilterSize, true); //texture0, 2(Local)

    //BRDF LUT
    MeshData texturePlane;//mesh 1
    texturePlane.SetVertexType(m_type);
    texturePlane.m_name = "TexturePlane";
    GeneratePlane(texturePlane);
    GenerateLUT(LUTSize, &texturePlane);//texture 1, 0(Local)
    m_meshVec.push_back(texturePlane);
}

void LightProbe::SampleCube(float face)
{
    //Bind effect
    m_meshVec[1].m_pEffect->BindEffect();
    //Update uniform value
    glUniform1f(glGetUniformLocation(m_meshVec[1].m_pEffect->getShaderProgramID(), "face"), face);
    //Bind cube map
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(m_meshVec[0].m_TextureVec[1].texID);
    texParaNameVec.push_back("cubeMap");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    m_meshVec[1].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

    m_meshVec[1].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[1].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void LightProbe::GenerateIrradianceMap(int numAzimuth, int numZenith, GLuint cubeID)
{
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Update uniform value
    glUniform1i(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "numAzimuth"), numAzimuth);
    glUniform1i(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "numZenith"), numZenith);
    //Bind cube map
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(cubeID);
    texParaNameVec.push_back("environmentMap");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void LightProbe::Prefilter(float roughness, GLuint cubeMapID)
{
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Update uniform value
    glUniform1f(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "roughness"), roughness);
    //Bind cube map
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(cubeMapID);
    texParaNameVec.push_back("environmentMap");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void LightProbe::IntegrateBRDF()
{
    //Bind effect
    m_meshVec[1].m_pEffect->BindEffect();
    
    m_meshVec[1].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[1].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void LightProbe::SetVolume(float radius)
{
    m_volumeRadius = radius;
}

void LightProbe::SetVolume(float length, float width, float height)
{
    m_volumeLength = length;
    m_volumeWidth = width;
    m_volumeHeight = height;
}

float LightProbe::ComputeWeight(vec3 pos)
{
    if (m_volumeShape == SPHERICAL)
    {
        return glm::clamp(1.0f - glm::distance(pos, GetTransform().GetPosition()) / m_volumeRadius, 0.0f, 1.0f);
    }
    else
    {
        //Compute volume local coordinate of POI point
        GetTransform().ComputeLocalToWorldMatrix();
        vec3 posL = glm::inverse(GetTransform().GetLoaclToWorldMatrix()) * vec4(pos, 1.0f);
        vec3 dirL = vec3(abs(posL.x), abs(posL.y), abs(posL.z));
        dirL = dirL * 2.0f / vec3(m_volumeLength, m_volumeHeight, m_volumeWidth);
        float effective = max(max(dirL.x, dirL.y), dirL.z);
        return glm::clamp(1.0f - effective, 0.0f, 1.0f);
    }
}