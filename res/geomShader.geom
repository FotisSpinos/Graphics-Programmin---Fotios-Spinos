#version 410

//Layout qualfier
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 transform;
uniform vec3 cameraPos;

in struct VertexData
{ 
	vec2 v_coord;
} VertexOutput[];

out struct GeometryData
{
	vec3 v_norm;
	vec3 v_pos; 
	vec2 v_coord;
} GeometryOutput;

vec3 recalculateNormals()
{
	vec4 a = transform * (gl_in[0].gl_Position - gl_in[1].gl_Position);
   	vec4 b = transform * (gl_in[2].gl_Position - gl_in[1].gl_Position);

   	return normalize(cross(vec3(a), vec3(b)));
}

bool showVertex(vec3 cameraDir, vec3 normal)
{	
	if(dot(cameraDir, normal) > 0.0)
		return true;

	return false;
}

void main()
{
	vec3 normal = recalculateNormals();

	for(int i = 0; i < 3; i++)
	{
		vec3 cameraDir = normalize(cameraPos - vec3(gl_in[i].gl_Position));

		if(showVertex(cameraDir, normal))
		{
			gl_Position = transform * gl_in[i].gl_Position;

			GeometryOutput.v_pos = vec3(gl_Position);
			GeometryOutput.v_norm = normal;//VertexOutput[i].v_norm;
			GeometryOutput.v_coord = VertexOutput[i].v_coord;

			EmitVertex();
		}
	}
	EndPrimitive();
}