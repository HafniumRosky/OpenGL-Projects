#include "ParticleSystem.h"

void ParticleSystem::LoadGameObject()
{
    m_pParticle = new Particle[m_maxParticle];

    m_pParticle[0].type = PARTICLE_TYPE_LAUNCHER;
    m_pParticle[0].pos = GetTransform().GetPosition();
    m_pParticle[0].vel = m_initiVel;
    m_pParticle[0].lifeTime = 0.0f;

    m_varyings[0] = "typeO";
    m_varyings[1] = "positionO";
    m_varyings[2] = "velocityO";
    m_varyings[3] = "ageO";

    GenerateBuffer();
    GenerateTexture();
    GenerateNoise();
}

void ParticleSystem::GenerateBuffer()
{
    glGenTransformFeedbacks(2, m_TFB);
    glGenBuffers(2, m_VBO);
    for (unsigned int i = 0; i < 2; i++) 
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TFB[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_maxParticle, m_pParticle, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[i]);
    }
}

void ParticleSystem::GenerateTexture()
{
    m_diffuse.type = "diffuse";
    SetUpTextureMapFromFile("../Project5\\Assets\\Textures\\Smoke", true, &m_diffuse, GL_MIRRORED_REPEAT);
}

void ParticleSystem::GenerateNoise()
{
    vec3* pRandomData = new vec3[m_noiseSize];

    for (unsigned int i = 0; i < m_noiseSize; i++) {
        pRandomData[i].x = rand() % (999) / (float)(1000);
        pRandomData[i].y = rand() % (999) / (float)(1000);
        pRandomData[i].z = rand() % (999) / (float)(1000);
    }

    glGenTextures(1, &m_noise.texID);
    glBindTexture(GL_TEXTURE_1D, m_noise.texID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, m_noiseSize, 0.0f, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete[] pRandomData;
}

void ParticleSystem::UpdateParticles()
{
    //Bind effect
    m_pUpdateEffect->BindEffect();
    //Set uniform values
    glUniform1f(glGetUniformLocation(m_pUpdateEffect->getShaderProgramID(), "dt"), m_dt);
    glUniform1f(glGetUniformLocation(m_pUpdateEffect->getShaderProgramID(), "time"), m_time);
    glUniform1f(glGetUniformLocation(m_pUpdateEffect->getShaderProgramID(), "launcherLifeTime"), m_launcherLifeTime);
    glUniform1f(glGetUniformLocation(m_pUpdateEffect->getShaderProgramID(), "shellLifeTime"), m_shellLifeTime);
    //Bind noise texture
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(m_noise.texID);
    texParaNameVec.push_back("noise");
    texTarget.push_back(GL_TEXTURE_1D);
    m_pUpdateEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
    //Discard primitives before rasterizing
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[m_currVBO]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TFB[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4); 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16); 
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28); 
    
    glBeginTransformFeedback(GL_POINTS);
    if (m_isFirst) 
    {
        glDrawArrays(GL_POINTS, 0, 1);
        m_isFirst = false;
    }
    else 
    {
        glDrawTransformFeedback(GL_POINTS, m_TFB[m_currVBO]);
    }

    glEndTransformFeedback();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void ParticleSystem::DrawParticles()
{
    //Bind effect
    m_pRenderEffect->BindEffect();
    //Bind texture
    std::vector<GLuint> texIDVec;
    std::vector<const char*> texParaNameVec;
    std::vector<GLenum> texTarget;
    texIDVec.push_back(m_diffuse.texID);
    texParaNameVec.push_back("smoke");
    texTarget.push_back(GL_TEXTURE_2D);
    m_pRenderEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
    //Disable the discard operation
    glDisable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[m_currTFB]);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4); // position

    glDrawTransformFeedback(GL_POINTS, m_TFB[m_currTFB]);

    glDisableVertexAttribArray(0);
}

void ParticleSystem::Draw()
{
    //GetTransform().ComputeLocalToWorldMatrix();
    UpdateParticles();
    DrawParticles();
    m_currVBO = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}

void ParticleSystem::Update(float dt)
{
    m_dt = dt;
    m_time += dt;
}