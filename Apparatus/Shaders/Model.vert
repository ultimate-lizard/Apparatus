#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

out vec2 texPos;
out vec3 fragPos;
out vec3 fragNormal;
out mat4 fragTransform;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * world * vec4(position, 1.0);
	
	texPos = uv;
	fragPos = vec3(world * vec4(position, 1.0));
	fragNormal = normal;
	fragTransform = world;
}
