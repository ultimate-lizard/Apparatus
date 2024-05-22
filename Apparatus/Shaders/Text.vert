#version 330 core

layout (location = 0) in vec3 positionLayout;
layout (location = 1) in vec2 texturePositionLayout;

out vec2 texturePosition;
out vec3 position;
out float textureArrayDepth;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(positionLayout.xy, 0.0f, 1.0);
	texturePosition = texturePositionLayout;
	textureArrayDepth = positionLayout.z;
}
