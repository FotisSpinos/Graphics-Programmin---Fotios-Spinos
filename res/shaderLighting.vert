//Version Number
#version 410

//The layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexCoord;

//Uniform variable
uniform mat4 transform;

//Passing out the normal and position data

out struct VertexData
{
	vec2 v_coord;
} VertexOutput;

void main()
{
	VertexOutput.v_coord = VertexCoord;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}

