#version 330 core

layout(origin_upper_left) in vec4 gl_FragCoord;

in vec2 texturePosition;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
	vec4 resultColor = texture(image, texturePosition);
	gl_FragColor = mix(resultColor, spriteColor, spriteColor.w);
}
