#include "Cube.h"

void Cube::GenerateCube(MeshData& cubeMesh)
{
    float side = 1.0f;
    float side2 = side / 2.0f;
    //Position
    // Front
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    // Right
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    // Back
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    // Left
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    // Bottom
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, side2));
    cubeMesh.m_posL.push_back(vec3(-side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, -side2));
    cubeMesh.m_posL.push_back(vec3(side2, -side2, side2));
    // Top
    cubeMesh.m_posL.push_back(vec3(-side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, side2));
    cubeMesh.m_posL.push_back(vec3(side2, side2, -side2));
    cubeMesh.m_posL.push_back(vec3(-side2, side2, -side2));

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
        cubeMesh.m_indexVec.push_back(index[i]);

    //Normal and Color
    // Front
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 0.0f, 1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Right
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(1.0f, 0.0f, 0.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Back
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 0.0f, -1.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Left
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(-1.0f, 0.0f, 0.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    //Bottom
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, -1.0f, 0.0f));
        cubeMesh.m_color.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    //Top
    for (int i = 0; i < 4; ++i)
    {
        cubeMesh.m_normalL.push_back(vec3(0.0f, 1.0f, 0.0f));
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


void Cube::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
    MeshData cubeMesh;
    cubeMesh.SetVertexType(m_type);
    cubeMesh.m_PBRMaterial.albedo = albedo;
    cubeMesh.m_PBRMaterial.metallic = metallic;
    cubeMesh.m_PBRMaterial.roughness = roughness;
    GenerateCube(cubeMesh);
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
    GetTransform().ComputeLocalToWorldMatrix();
    //Bind effect
    m_meshVec[0].m_pEffect->BindEffect();
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