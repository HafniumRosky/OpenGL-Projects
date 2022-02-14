#version 430

uniform sampler2D smoke;

in vec2 uv;
out vec4 FragColor;

void main()
{
    FragColor = vec4(texture2D(smoke, uv).a);
}