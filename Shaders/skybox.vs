#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 View;
uniform mat4 Projection;

out vec3 texCoords;

void main()
{
	vec4 pos = Projection * View * vec4(aPos, 1.0);
	
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
}