#version 430

in v2f
{
	vec3 texcoord;
}pIn;
  
uniform samplerCube environmentMap;
 
out vec4 FragColor;

void main()
{
    vec3 envColor = texture(environmentMap, pIn.texcoord).rgb;
    
    //envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
}