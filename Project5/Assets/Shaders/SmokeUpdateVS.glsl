#version 430

layout (location = 0) in float type;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 velocity;
layout (location = 3) in float age;

out float typeG;
out vec3 positionG;
out vec3 velocityG;
out float ageG;

void main()
{
    typeG = type;
    positionG = position;
    velocityG = velocity;
    ageG = age;
}