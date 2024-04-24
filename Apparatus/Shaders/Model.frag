#version 330 core

out vec4 FragColor;
in vec2 texPos;
in vec3 fragPos;
in vec3 fragNormal;
in mat4 fragTransform;

// From light
uniform vec3 cameraPos;

struct Material
{
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;

	bool selected;
	vec4 selectionColor;

	vec3 color;
};


struct Light
{
	vec3 color;
	float compression;

	vec3 position;
	float radius;

	vec3 direction;
	float cutOff;

	float outerCutOff;
	int type;
};

#define LIGHT_NUM_MAX 128

uniform Material material;

layout (std140) uniform LightUniformBuffer
{
	int lightNum;
	Light lights[LIGHT_NUM_MAX];
};

float near = 10.0;
float far = 10000.0;

vec3 calculateDirectionalLight(Light inLight, Material inMaterial);
vec3 calculatePointLight(Light inLight, Material inMaterial);
vec3 calculateSpotLight(Light inLight, Material inMaterial);

float linearizeDepth(float depth);

vec3 fragNormalWorld;

void main()
{
	fragNormalWorld = normalize(fragTransform * vec4(fragNormal, 0.0)).xyz;

	vec4 objectColor = texture(material.diffuseMap, texPos);

	vec3 resultColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < lightNum; ++i)
	{
		if (lights[i].type == 0)
		{
			resultColor += calculateDirectionalLight(lights[i], material) * vec3(objectColor);
		}
		else if (lights[i].type == 1)
		{
			resultColor += calculatePointLight(lights[i], material) * vec3(objectColor);
		}
		else
		{
			resultColor += calculateSpotLight(lights[i], material) * vec3(objectColor);
		}
	}

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

vec3 calculateDirectionalLight(Light inLight, Material inMaterial)
{
	vec3 ambientColor = inLight.color * vec3(texture(inMaterial.diffuseMap, texPos));

	vec3 lightDir = normalize(-inLight.direction);

	float diffuseValue = max(dot(fragNormalWorld, lightDir), 0.0);
	vec3 diffuseColor = inLight.color * (diffuseValue * vec3(texture(inMaterial.diffuseMap, texPos)));

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormalWorld);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));
	vec3 specularColor = inLight.color * (specularValue * inMaterial.specular);

	return ambientColor + diffuseColor + specularColor;
}

vec3 calculatePointLight(Light inLight, Material inMaterial)
{
	vec3 lightDir = normalize(inLight.position - fragPos);
	float diffuseValue = max(dot(fragNormalWorld, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormalWorld);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));

	float attenuation = pow(smoothstep(inLight.radius, 0, length(inLight.position - fragPos)), inLight.compression);

	vec3 ambientColor = inLight.color * vec3(texture(inMaterial.diffuseMap, texPos));
	vec3 diffuseColor = inLight.color * (diffuseValue * vec3(texture(inMaterial.diffuseMap, texPos)));
	vec3 specularColor = inLight.color * (specularValue * inMaterial.specular);

	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specularColor *= attenuation;

	return ambientColor + diffuseColor + specularColor;
}

vec3 calculateSpotLight(Light inLight, Material inMaterial)
{
	vec3 lightDir = normalize(inLight.position - fragPos);
	float diffuseValue = max(dot(fragNormalWorld, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormalWorld);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));

	float attenuation = pow(smoothstep(inLight.radius, 0, length(inLight.position - fragPos)), inLight.compression);

	float theta = dot(lightDir, normalize(-inLight.direction));
	float epsilon = inLight.cutOff - inLight.outerCutOff;
	float intensity = clamp((theta - inLight.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambientColor = inLight.color * vec3(texture(inMaterial.diffuseMap, texPos));
	vec3 diffuseColor = inLight.color * (diffuseValue * vec3(texture(inMaterial.diffuseMap, texPos)));
	vec3 specularColor = inLight.color * (specularValue * inMaterial.specular);

	ambientColor *= attenuation * intensity;
	diffuseColor *= attenuation * intensity;
	specularColor *= attenuation * intensity;

	return ambientColor + diffuseColor + specularColor;
}


float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
