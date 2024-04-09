#version 330 core

in vec4 vertColor;

uniform bool selected;
uniform vec4 selectionColor;

float linearizeDepth(float depth);

float near = 10.0;
float far = 10000.0;

void main()
{
	if (selected)
	{
		gl_FragColor = selectionColor;
	}
	else
	{
		gl_FragColor = vertColor;
	}

	float depth = linearizeDepth(gl_FragCoord.z) / far;
	gl_FragDepth = depth;
}

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
