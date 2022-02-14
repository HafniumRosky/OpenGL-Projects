#version 430

in v2f
{
	vec3 posL;
}pIn;

uniform sampler2D SphericalMap;

out vec4 FragColor;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(pIn.posL)); // make sure to normalize localPos
    vec3 color = texture(SphericalMap, uv).rgb;
    FragColor = vec4(color, 1.0);
}  