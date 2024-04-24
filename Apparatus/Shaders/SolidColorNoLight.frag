#version 330 core

in vec4 vertColor;

float linearizeDepth(float depth);

float near = 10.0;
float far = 10000.0;

struct Material
{
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;

	bool selected;
	vec4 selectionColor;

	vec3 color;
};

uniform Material material;

void main()
{
	vec4 resultColor = vec4(material.color, 1.0);

	if (material.selected)
	{
		gl_FragColor = mix(resultColor, material.selectionColor, 0.5);
	}
	else
	{
		gl_FragColor = resultColor;
	}

	float depth = linearizeDepth(gl_FragCoord.z) / far;
	gl_FragDepth = depth;
}

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
