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
        std::string path = "../Project5\\Assets\\Precomputation\\Probe" + m_lightPorbeName + "\\DualParaboloidFront.hdr";
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
        std::string path = "../Project5\\Assets\\Precomputation\\Probe" + m_lightPorbeName + "\\DualParaboloidBack.hdr";
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

void LightProbe::LoadLightProbe(GLsizei irrWidth, GLsizei irrHeight, GLsizei prefilterSize)
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
    GenerateEmptyCubeMap(m_meshVec[0], irrWidth, irrHeight); //texture0, 2

    //Prifilter map
    GenerateEmptyCubeMap(m_meshVec[0], prefilterSize, prefilterSize, true); //texture0, 3
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

void LightProbe::GenerateIrradianceMap(int numAzimuth, int numZenith)
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
    texIDVec.push_back(m_meshVec[0].m_TextureVec[1].texID);
    texParaNameVec.push_back("environmentMap");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void LightProbe::Prefilter(float roughness)
{
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Update uniform value
    glUniform1i(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "roughness"), roughness);
    //Bind cube map
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(m_meshVec[0].m_TextureVec[1].texID);
    texParaNameVec.push_back("environmentMap");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);

    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}