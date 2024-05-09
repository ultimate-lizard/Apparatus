#version 330 core

in vec2 texturePosition;

uniform sampler2D image;

uniform vec4 borders;
uniform vec2 textureBorderSize;
uniform vec2 spriteSize;
uniform vec4 spriteColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
	vec2 texSize = textureBorderSize;

	float left = borders.x;
	float right = borders.y;
	float top = borders.z;
	float bottom = borders.w;

	vec2 texPos = texturePosition * texSize;
	vec2 texturePositionShifted = texPos;

	vec2 patchValue;
	vec2 patchStart;
	vec2 patchEnd;

	// Top Left
	if (texPos.x <= left && texPos.y <= top)
	{
		gl_FragColor = texture(image, texturePositionShifted / texSize);	
		return;
	}
	// Top Right
	else if (texPos.x >= spriteSize.x - left && texPos.y <= top)
	{
		texturePositionShifted.x = texturePositionShifted.x - spriteSize.x;
		gl_FragColor = texture(image, texturePositionShifted / texSize);	
		return;
	}
	// Bottom left
	else if (texPos.x <= left && texPos.y >= spriteSize.y - top)
	{
		texturePositionShifted.y = texturePositionShifted.y - spriteSize.y;
		gl_FragColor = texture(image, texturePositionShifted / texSize);
		return;
	}
	// Bottom right
	else if (texPos.x >= spriteSize.x - left && texPos.y >= spriteSize.y - top)
	{
		// I don't really know why this offsetting works, but there might be a more logical one
		texturePositionShifted.y = texturePositionShifted.y - spriteSize.y;
		texturePositionShifted.x = texturePositionShifted.x - spriteSize.x;
		gl_FragColor = texture(image, texturePositionShifted / texSize);
		return;
	}
	// Top
	else if (texPos.x >= left && texPos.y <= top && texPos.x <= spriteSize.x - left)
	{
		texturePositionShifted.x = texturePositionShifted.x - left;

		vec2 patchSize = vec2(texSize.x - left - right, top);
		patchValue = texturePositionShifted / patchSize;

		patchStart = vec2(left, 0.0);
		patchEnd = vec2(texSize.x - left, top);
	}
	// Bottom
	else if (texPos.x >= left && texPos.x <= spriteSize.x - left && texPos.y >= spriteSize.y - top)
	{
		texturePositionShifted.x = texturePositionShifted.x - left;
		texturePositionShifted.y = texturePositionShifted.y - spriteSize.y - top;

		vec2 patchSize = vec2(texSize.x - left - right, bottom);
		patchValue = texturePositionShifted / patchSize;

		patchStart = vec2(left, texSize.y - top);
		patchEnd = vec2(texSize.x - left, texSize.y);
	}
	// Left
	else if (texPos.x <= left && texPos.y >= top && texPos.y <= spriteSize.y - top)
	{
		texturePositionShifted.y = texturePositionShifted.y - top;

		vec2 patchSize = vec2(left, texSize.y - top - bottom);
		patchValue = texturePositionShifted / patchSize;

		patchStart = vec2(0.0f, top);
		patchEnd = vec2(left, texSize.y - top);
	}
	// Right
	else if (texPos.x >= spriteSize.x - left && texPos.y >= top && texPos.y <= spriteSize.y - top)
	{
		texturePositionShifted.x = texturePositionShifted.x - spriteSize.x - left;
		texturePositionShifted.y = texturePositionShifted.y - top;

		vec2 patchSize = vec2(right, texSize.y - top - bottom);
		patchValue = texturePositionShifted / patchSize;

		patchStart = vec2(texSize.x - left, top);
		patchEnd = vec2(texSize.x, texSize.y - top);
	}
	// Center
	else if (texPos.x >= left && texPos.x <= spriteSize.x - left && texPos.y >= top && texPos.y <= spriteSize.y - top)
	{
		texturePositionShifted.x = texturePositionShifted.x - left;
		texturePositionShifted.y = texturePositionShifted.y - top;

		vec2 patchSize = vec2(texSize.x - left - right, texSize.y - top - bottom);
		patchValue = texturePositionShifted / patchSize;

		patchStart = vec2(left, top);
		patchEnd = vec2(texSize.x - left, texSize.y - top);
	}

	float mappedX = map(fract(patchValue.x), 0.0, 1.0, patchStart.x / texSize.x, patchEnd.x / texSize.x);
	float mappedY = map(fract(patchValue.y), 0.0, 1.0, patchStart.y / texSize.y, patchEnd.y / texSize.y);

	vec4 resultColor = texture(image, vec2(mappedX, mappedY));
	gl_FragColor = mix(resultColor, spriteColor, spriteColor.w);
}
