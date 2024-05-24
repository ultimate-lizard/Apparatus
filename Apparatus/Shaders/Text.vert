#version 330 core

layout (location = 0) in vec2 positionLayout;
layout (location = 1) in vec3 texturePositionLayout;

out vec3 texturePosition;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(positionLayout.xy, 0.0f, 1.0);
	texturePosition = texturePositionLayout;
}
