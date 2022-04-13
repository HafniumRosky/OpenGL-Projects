#include "Cube.h"

void Cube::GenerateCube(MeshData& cubeMesh)
{
    float side = 1.0f;
    float side2 = side / 2.0f;
    //Position
    // right
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    // left
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    // top
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    // bottom
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    // back
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    // front
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));

    //Index
    GLuint index[36] = {
        0,1,2,2,3,0,
        4,5,6,6,7,4,
        8,9,10,10,11,8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20
    };
    for (int i = 0; i < 36; i++)
        cubeMesh.m_indexVec.push_back(index[i]);

    //Normal and Color
    // right
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(1.0f, 0.0f, 0.0f));
        cubeMesh.m_tangent.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // left
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(-1.0f, 0.0f, 0.0f));
        cubeMesh.m_tangent.push_back(vec4(0.0f, 0.0f, -1.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // top
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 1.0f, 0.0f));
        cubeMesh.m_tangent.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // bottom
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, -1.0f, 0.0f));
        cubeMesh.m_tangent.push_back(vec4(-1.0f, 0.0f, 0.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    //back
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 0.0f, 1.0f));
        cubeMesh.m_tangent.push_back(vec4(-1.0f, 0.0f, 0.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    //front
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 0.0f, -1.0f));
        cubeMesh.m_tangent.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void Cube::LoadGameObject(float gloss)
{
    MeshData cubeMesh;
    cubeMesh.SetVertexType(m_type);
    GenerateCube(cubeMesh);
    m_meshVec.push_back(cubeMesh);
    m_meshVec[0].m_material.ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    m_meshVec[0].m_material.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_meshVec[0].m_material.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_meshVec[0].m_material.gloss = gloss;
}

void Cube::LoadPBRTextures(MeshData& cubeMesh, bool loadTex)
{
    Texture albedoMap;
    albedoMap.name = m_materialName + "_basecolor.png";
    albedoMap.type = "albedo";
    Texture metallicMap;
    metallicMap.name = m_materialName + "_metallic.png";
    metallicMap.type = "metallic";
    Texture roughnessMap;
    roughnessMap.name = m_materialName + "_roughness.png";
    roughnessMap.type = "roughness";


    if (loadTex)
    {
        SetUpTextureMapFromFile(m_texturePath, true, &albedoMap, GL_MIRRORED_REPEAT);
        SetUpTextureMapFromFile(m_texturePath, true, &metallicMap, GL_MIRRORED_REPEAT);
        SetUpTextureMapFromFile(m_texturePath, true, &roughnessMap, GL_MIRRORED_REPEAT);
    }

    cubeMesh.m_TextureVec.push_back(albedoMap);
    cubeMesh.m_TextureVec.push_back(metallicMap);
    cubeMesh.m_TextureVec.push_back(roughnessMap);

    if (m_loadNormal)
    {
        Texture normalMap;
        normalMap.name = m_materialName + "_normal.png";
        normalMap.type = "normal";
        SetUpTextureMapFromFile(m_texturePath, true, &normalMap, GL_MIRRORED_REPEAT);
        cubeMesh.m_TextureVec.push_back(normalMap);
    }

    if (m_loadAO)
    {
        Texture aoMap;
        aoMap.name = m_materialName + "_ao.png";
        aoMap.type = "AO";
        SetUpTextureMapFromFile(m_texturePath, true, &aoMap, GL_MIRRORED_REPEAT);
        cubeMesh.m_TextureVec.push_back(aoMap);
    }
}

void Cube::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
    MeshData cubeMesh;
    cubeMesh.SetVertexType(m_type);
    cubeMesh.m_PBRMaterial.albedo = albedo;
    cubeMesh.m_PBRMaterial.metallic = metallic;
    cubeMesh.m_PBRMaterial.roughness = roughness;
    GenerateCube(cubeMesh);
    bool loadTex = true;
    if (albedo.x >= 0)
        loadTex = false;
    LoadPBRTextures(cubeMesh, loadTex);
    m_meshVec.push_back(cubeMesh);
}

void Cube::Draw()
{
    GetTransform().ComputeLocalToWorldMatrix();
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Update Constant Buffer(change every draw)
    //World matrix
    Effect::cbDraw.world = GetMeshWorld(0);
    //World inverse matrix
    Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(0);
    //Materials
    Effect::cbDraw.material = m_meshVec[0].m_material;
    glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Cube::DrawPBR()
{
    this->GetTransform().ComputeLocalToWorldMatrix();
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
    //Bind texture
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(m_meshVec[0].m_TextureVec[0].texID);
    texIDVec.push_back(m_meshVec[0].m_TextureVec[1].texID);
    texIDVec.push_back(m_meshVec[0].m_TextureVec[2].texID);
    texParaNameVec.push_back("albedoMap");
    texParaNameVec.push_back("metallicMap");
    texParaNameVec.push_back("roughnessMap");
    texTarget.push_back(GL_TEXTURE_2D);
    texTarget.push_back(GL_TEXTURE_2D);
    texTarget.push_back(GL_TEXTURE_2D);
    //test
    texIDVec.push_back(m_meshVec[0].m_TextureVec[3].texID);
    texIDVec.push_back(m_meshVec[0].m_TextureVec[4].texID);
    texIDVec.push_back(m_meshVec[0].m_TextureVec[5].texID);
    texParaNameVec.push_back("irradianceMap");
    texParaNameVec.push_back("prefilteredMap");
    texParaNameVec.push_back("BRDFLUT");
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    texTarget.push_back(GL_TEXTURE_CUBE_MAP);
    texTarget.push_back(GL_TEXTURE_2D);

    //Bind uniform
    glUniform4fv(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "emmision"), 1, &m_emmision[0]);

    if (m_loadNormal)
    {
        texIDVec.push_back(m_meshVec[0].m_TextureVec[3].texID);
        texParaNameVec.push_back("normalMap");
        texTarget.push_back(GL_TEXTURE_2D);
    }
    m_meshVec[0].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
    //Update Constant Buffer(change every draw)
    //World matrix
    Effect::cbDrawPBR.world = GetMeshWorld(0);
    //World inverse matrix
    Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(0);
    //Materials
    Effect::cbDrawPBR.material = m_meshVec[0].m_PBRMaterial;
    glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[5]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_meshVec[0].BindVertexArray();
    glDrawElements(GL_TRIANGLES, m_meshVec[0].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Cube::Update(float dt)
{
    GetTransform().Rotate(0.2f * dt, vec3(0.0f, 1.0f, 0.0f));
}