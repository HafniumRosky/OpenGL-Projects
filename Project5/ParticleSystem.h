#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "GameObject.h"

struct Particle
{
    float type;
    vec3 pos;
    vec3 vel;
    float lifeTime;
};

enum ParticleType
{
    PARTICLE_TYPE_LAUNCHER = 1,
    PARTICLE_TYPE_SHELL = 2
};

class ParticleSystem : public GameObject
{
private:
    //Particles
    Particle* m_pParticle;

    //Buffer
    GLuint m_VBO[2];
    GLuint m_TFB[2];
    int m_currVBO = 0;
    int m_currTFB = 1;

    //Properties
    int m_maxParticle;
    vec3 m_initiVel;
    float m_time = 0.0f;
    float m_dt = 0.0f;
    float m_launcherLifeTime;
    float m_shellLifeTime;
    bool m_isFirst = true;

    //Texture
    Texture m_diffuse;
    Texture m_noise;
    GLsizei m_noiseSize = 1024;

    //Effect
    Effect* m_pUpdateEffect = nullptr;
    Effect* m_pRenderEffect = nullptr;

    //Varyings
    const GLchar* m_varyings[4];

    void GenerateBuffer();
    void GenerateTexture();
    void GenerateNoise();
    void DrawParticles();
    void UpdateParticles();
public:
	ParticleSystem() = default;
    ParticleSystem(float launcherLifeTime, float shellLifeTime, std::string texFileName, int maxNum)
    {
        m_launcherLifeTime = launcherLifeTime;
        m_shellLifeTime = shellLifeTime;
        m_diffuse.name = texFileName;
        m_maxParticle = maxNum;
    }
	~ParticleSystem() 
    {
        delete m_pParticle;
        delete m_pUpdateEffect;
        delete m_pRenderEffect;
    }

    void LoadGameObject();
    void Draw();
    void Update(float dt);

    void SetInitialVelocity(vec3 vel)
    {
        m_initiVel = vel;
    }

    void SetUpdateEffect(Effect* pEffect)
    {
        m_pUpdateEffect = pEffect;
    }

    void SetRenderEffect(Effect* pEffect)
    {
        m_pRenderEffect = pEffect;
    }

    const GLchar** GetVarings()
    {
        return m_varyings;
    }
};


#endif