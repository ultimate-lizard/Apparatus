#version 330 core

out vec4 col;

in vec3 texturePosition;

uniform sampler2DArray image;
uniform vec4 spriteColor;

void main()
{
	vec4 textChannel = vec4(1.0, 1.0, 1.0, texture(image, vec3(texturePosition.xy, texturePosition.z)).r);
	col = spriteColor * textChannel;
}
