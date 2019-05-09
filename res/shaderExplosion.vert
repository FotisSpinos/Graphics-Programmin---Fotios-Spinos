#version 410

//The layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 textureCoordinate;

//Uniform variable
uniform mat4 transform;

//Passing out the normal and position data
out VertexData
{
	out vec2 texCoord;
} VertexInput;

void main()
{
	//Assigning the normal and position data
	VertexInput.texCoord = textureCoordinate;

	// Sets the position of the current vertex
	gl_Position = transform * vec4(VertexPosition, 1.0);
}