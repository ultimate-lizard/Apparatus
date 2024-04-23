#version 330 core

in vec4 vertColor;

//uniform bool selected;
//uniform vec4 selectionColor;

float linearizeDepth(float depth);

float near = 10.0;
float far = 10000.0;

struct Material
{
	// Phong
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	bool selected;
	bool noTexture;

	vec4 selectionColor;
	vec4 backupColor;
};

uniform Material material;

void main()
{
	vec4 objectColor = material.backupColor;
	vec3 resultColor = material.diffuse * vec3(objectColor);

	if (material.selected)
	{
		gl_FragColor = mix(vec4(resultColor, 1.0), material.selectionColor, 0.5);
	}
	else
	{
		gl_FragColor = vec4(resultColor, 1.0);
	}

	float depth = linearizeDepth(gl_FragCoord.z) / far;
	gl_FragDepth = depth;
}

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
