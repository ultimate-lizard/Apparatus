#version 330 core

out vec4 color;
in vec2 texPos;

uniform sampler2D textureSampler;
uniform bool selected;
uniform vec4 selectionColor;

void main()
{
	if (selected)
	{
		color = mix(texture(textureSampler, texPos), selectionColor, 0.5f);
	}
	else
	{
		color = texture(textureSampler, texPos);
	}
}
