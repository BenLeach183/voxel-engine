#version 330 core
layout(location = 0) in uint aPackedData;

vec3 position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 normal;
out vec3 posInWS;
out vec2 uv;

#include "unpack_voxel_vertices.glsl"

void main()
{
	unpackData();
	
	vec4 worldSpace = Model * vec4(position, 1.0);	// vertex in world space as vec4
	posInWS = vec3(worldSpace.xyz);					// vertex in world space as vec3
	
    gl_Position = Projection * View * worldSpace;
}