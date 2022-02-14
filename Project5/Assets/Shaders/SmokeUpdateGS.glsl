#version 430

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in float typeG[];
in vec3 positionG[];
in vec3 velocityG[];
in float ageG[];

out float typeO;
out vec3 positionO;
out vec3 velocityO;
out float ageO;

uniform float dt;
uniform float time;
uniform float launcherLifeTime;
uniform float shellLifeTime;

uniform sampler1D noise;

vec3 GetRandomDir(float t)
{
    vec3 Dir = texture(noise, t).xyz;
    Dir -= vec3(0.5, 0.5, 0.5);
    return Dir;
}

void main()
{
    float age = ageG[0] + dt;

    if (typeG[0] == 1.0) 
    {
        if (age >= launcherLifeTime) 
        {
            typeO = 2;
            positionO = positionG[0];
            vec3 Dir = GetRandomDir(time);
            Dir.y = max(Dir.y, 0.5);
            velocityO = normalize(Dir) * 20;
            ageO = 0.0;
            EmitVertex();
            EndPrimitive();
            age = 0.0;
        }

        typeO = 1;
        positionO = positionG[0];
        velocityO = velocityG[0];
        ageO = age;
        EmitVertex();
        EndPrimitive();
    }
    else
    {
        vec3 DeltaP = dt * velocityG[0];
        vec3 DeltaV = vec3(dt) * (0.0, -5000.0, 0.0);
        if (age < shellLifeTime) 
        {
                typeO = 2;
                positionO = positionG[0] + DeltaP;
                velocityO = velocityG[0] + DeltaV;
                ageO = age;
                EmitVertex();
                EndPrimitive();
        }
    }
}