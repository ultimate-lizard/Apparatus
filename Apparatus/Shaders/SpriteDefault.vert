#version 330 core

layout (location = 0) in vec3 positionLayout;
layout (location = 1) in vec2 texturePositionLayout;
layout (location = 2) in vec4 spritecolorLayout;

out vec2 texturePosition;
out vec4 spriteColor;
out vec3 position;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(positionLayout, 1.0);
	texturePosition = texturePositionLayout;
	spriteColor = spritecolorLayout;
}
