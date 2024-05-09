#version 330 core

layout(origin_upper_left) in vec4 gl_FragCoord;

in vec2 texturePosition;
in float textureArrayDepth;

uniform sampler2DArray image;
uniform vec4 spriteColor;

void main()
{
	float textChannel = texture(image, vec3(texturePosition, textureArrayDepth)).r;
	vec4 resultColor = vec4(1.0, 1.0, 1.0, textChannel);

	float cachedAlpha = resultColor.w;

	gl_FragColor = mix(resultColor, spriteColor, spriteColor.w);

	if (cachedAlpha < 0.1f)
	{
		discard;
	}
}
