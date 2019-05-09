#version 410

//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
layout (location = 1) in vec2 textureCoordinate;

in GeometryData
{
	vec2 texCoord;
} GeometryOutput;

uniform sampler2D texture;
uniform float texCont;

void main()
{
	fragcolor = texture2D(texture, GeometryOutput.texCoord) * texCont;
}