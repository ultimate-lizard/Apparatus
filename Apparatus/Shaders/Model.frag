#version 330 core

out vec4 color;
in vec2 texPos;

uniform sampler2D textureSampler;

uniform bool selected;
uniform bool noTexture;

uniform vec4 selectionColor;
uniform vec4 backupColor;

void main()
{
	if (noTexture)
	{
		if (selected)
		{
			color = vec4(selectionColor.xyz, 1.0);
		}
		else
		{
			color = backupColor;
		}
	}
	else
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
}
