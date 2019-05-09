#version 410

//Layout Qualifer
out vec4 fragcolor;

uniform vec3 cameraPos;
uniform vec3 lightPos;

// Light colours
uniform vec3 aColor;
uniform vec3 dColor;
uniform vec3 sColor;

// Light intensity
uniform float aIntensity;
uniform float dIntensity;
uniform float sIntensity;

uniform float sRoughness;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float texCont1;
uniform float texCont2;

in struct GeometryData
{
	vec3 v_norm;
	vec3 v_pos; 
	vec2 v_coord;
} GeometryOutput;

vec3 ambient()
{
	vec3 result = aIntensity * aColor;		
	return result;
}

vec3 diffuse(vec3 lightDir, vec3 v_norm)
{
	float dCont = max(0.0, dot(v_norm, lightDir));

	return (dColor * dCont * dIntensity);
}

vec3 specular(vec3 lightDir, vec3 v_norm, vec3 v_pos)
{
	vec3 viewDir = normalize(v_pos - cameraPos);

	vec3 reflectionDir = reflect(lightDir, v_norm);
	float dotViewReflection = dot(viewDir, reflectionDir);

	float sCont = pow(max(0.0, dotViewReflection), sRoughness);
	
	return (sCont * sIntensity * sColor);
}

void main()
{
	vec3 lightToVert = lightPos - GeometryOutput.v_pos;
	vec3 lightDir = normalize(lightToVert);
	float lightDist = length(lightToVert);
	float distF = 1 / lightDist;

	vec3 a = ambient();
	vec3 s = specular(lightDir, GeometryOutput.v_norm, GeometryOutput.v_pos) * distF;
	vec3 d = diffuse(lightDir, GeometryOutput.v_norm) * distF;

	vec4 texture1Color = texture2D(texture1, GeometryOutput.v_coord) * texCont1;
	vec4 texture2Color = texture2D(texture2, GeometryOutput.v_coord) * texCont2;

    fragcolor = lightColor + texture1Color + texture2Color;
}