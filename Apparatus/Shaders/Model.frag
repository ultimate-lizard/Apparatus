#version 330 core

out vec4 FragColor;
in vec2 texPos;
in vec3 fragPos;
in vec3 fragNormal;
in mat4 fragTransform;

uniform sampler2D textureSampler;

// From light
uniform vec3 cameraPos;

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

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	float compression;

	vec3 position;
	float radius;
};

struct SpotLight
{
//	vec3 color;
//	float compression;
//
//	vec3 position;
//	float radius;

	vec3 direction;
	float cutOff;
	float outerCutOff;
};

#define LIGHT_NUM_MAX 128

uniform Material material;

uniform int directionalLightNum;
uniform DirectionalLight directionalLight[LIGHT_NUM_MAX];

//uniform int spotLightNum;
//uniform SpotLight spotLight[LIGHT_NUM_MAX];

layout (std140) uniform LightUniformBuffer
{
	int pointLightNum;
	PointLight pointLight[LIGHT_NUM_MAX];
};

float near = 10.0;
float far = 10000.0;

vec3 calculateDirectionalLight(DirectionalLight inLight, Material inMaterial);
vec3 calculatePointLight(PointLight inLight, Material inMaterial);
// vec3 calculateSpotLight(SpotLight inLight, Material inMaterial);

float linearizeDepth(float depth);

vec3 fragNormalWorld;

void main()
{
	// fragNormal = normalize(fragTransform * fragNormal).xyz;
	fragNormalWorld = normalize(fragTransform * vec4(fragNormal, 0.0)).xyz;

	vec4 objectColor = texture(textureSampler, texPos);

	vec3 resultColor = vec3(0.0, 0.0, 0.0);

	DirectionalLight l;
	l.color = vec3(0.1, 0.08, 0.04);
	l.direction = vec3(1.0, -0.5, 1.0);
	resultColor += calculateDirectionalLight(l, material) * vec3(objectColor);
	// Lighting mixing
//	for (int i = 0; i < directionalLightNum; ++i)
//	{
//		resultColor += calculateDirectionalLight(directionalLight[i], material) * vec3(objectColor);
//	}

	for (int i = 0; i < pointLightNum; ++i)
	{
		resultColor += calculatePointLight(pointLight[i], material) * vec3(objectColor);
	}

//	for (int i = 0; i < spotLightNum; ++i)
//	{
//		resultColor += calculateSpotLight(spotLight[i], material) * vec3(objectColor);
//	}

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

vec3 calculateDirectionalLight(DirectionalLight inLight, Material inMaterial)
{
	vec3 ambientColor = inLight.color * vec3(texture(textureSampler, texPos));

	vec3 lightDir = normalize(-inLight.direction);

	float diffuseValue = max(dot(fragNormalWorld, lightDir), 0.0);
	vec3 diffuseColor = inLight.color * (diffuseValue * inMaterial.diffuse);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormalWorld);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));
	vec3 specularColor = inLight.color * (specularValue * inMaterial.specular);

	return ambientColor + diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight inLight, Material inMaterial)
{
	vec3 lightDir = normalize(inLight.position - fragPos);
	float diffuseValue = max(dot(fragNormalWorld, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormalWorld);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));

	// Attenuation
	float attenuation = pow(smoothstep(inLight.radius, 0, length(inLight.position - fragPos)), inLight.compression);

	vec3 ambientColor = inLight.color * vec3(texture(textureSampler, texPos));
	vec3 diffuseColor = inLight.color * (diffuseValue * inMaterial.diffuse);
	vec3 specularColor = inLight.color * (specularValue * inMaterial.specular);

	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specularColor *= attenuation;

	return ambientColor + diffuseColor + specularColor;
}

//vec3 calculateSpotLight(SpotLight inLight, Material inMaterial)
//{
//	vec3 lightDir = normalize(inLight.pointLight.position - fragPos);
//	float diffuseValue = max(dot(fragNormal, lightDir), 0.0);
//
//	vec3 viewDir = normalize(cameraPos - fragPos);
//	vec3 reflectionDir = reflect(-lightDir, fragNormal);
//	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));
//
//	// Attenuation
//	float dist = length(inLight.pointLight.position - fragPos);
//	// float attenuation = 1.0 / (inLight.constant + inLight.linear * dist + inLight.quadratic * (dist * dist));
//	float attenuation = pow(smoothstep(inLight.pointLight.radius, 0, length(inLight.pointLight.position - fragPos)), inLight.pointLight.compression);
//
//	float theta = dot(lightDir, normalize(-inLight.direction));
//	float epsilon = inLight.cutOff - inLight.outerCutOff;
//	float intensity = clamp((theta - inLight.outerCutOff) / epsilon, 0.0, 1.0);
//
//	vec3 ambientColor = inLight.light.color * vec3(texture(textureSampler, texPos));
//	vec3 diffuseColor = inLight.light.color * (diffuseValue * inMaterial.diffuse);
//	vec3 specularColor = inLight.light.color * (specularValue * inMaterial.specular);
//
//	ambientColor *= attenuation * intensity;
//	diffuseColor *= attenuation * intensity;
//	specularColor *= attenuation * intensity;
//
//	return ambientColor + diffuseColor + specularColor;
//}

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
