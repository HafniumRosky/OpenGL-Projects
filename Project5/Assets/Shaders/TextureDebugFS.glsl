#version 430

in vec2 uv;

uniform sampler2D tex;
uniform sampler2D texWithMip;

uniform int useMipMap = 0;
uniform float textureScale = 1.0;

out vec4 FragColor;

void main()
{
	if(useMipMap == 0)
		FragColor = texture(tex, uv * textureScale);
	else
		FragColor = texture(texWithMip, uv * textureScale);
}