#version 330 core

// layout(origin_upper_left) in vec4 gl_FragCoord;

out vec4 col;

in vec2 texturePosition;
in float textureArrayDepth;

uniform sampler2DArray image;
uniform vec4 spriteColor;

void main()
{
	vec4 textChannel = vec4(1.0, 1.0, 1.0, texture(image, vec3(texturePosition, textureArrayDepth)).r);
	// vec4 resultColor = vec4(1.0, 1.0, 1.0, textChannel);

	// float cachedAlpha = resultColor.w;

	col = spriteColor * textChannel;

//
//	if (col.a == 0.0f)
//	{
//		discard;
//	}
}
