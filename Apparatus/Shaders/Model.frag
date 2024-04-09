#version 330 core

out vec4 FragColor;
in vec2 texPos;
in vec3 fragPos;
in vec3 fragNormal;

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

struct LightBase
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	LightBase light;

	vec3 direction;
};

struct PointLight
{
	LightBase light;

	vec3 position;

	float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
	LightBase light;

	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
    float linear;
    float quadratic;
};

#define LIGHT_NUM_MAX 8

uniform Material material;

uniform int directionalLightNum;
uniform DirectionalLight directionalLight[LIGHT_NUM_MAX];

uniform int pointLightNum;
uniform PointLight pointLight[LIGHT_NUM_MAX];

uniform int spotLightNum;
uniform SpotLight spotLight[LIGHT_NUM_MAX];

vec3 calculateDirectionalLight(DirectionalLight inLight, Material inMaterial);
vec3 calculatePointLight(PointLight inLight, Material inMaterial);
vec3 calculateSpotLight(SpotLight inLight, Material inMaterial);

void main()
{
	vec4 objectColor;
	
	if (material.noTexture)
	{
		objectColor = material.backupColor;
	}
	else
	{
		objectColor = texture(textureSampler, texPos);
	}

	vec3 resultColor = vec3(0.0, 0.0, 0.0);

	if (material.noTexture)
	{
		// No lighting
		resultColor = (material.diffuse + material.ambient + material.specular) * vec3(objectColor);
	}
	else
	{
		// Lighting mixing
		for (int i = 0; i < directionalLightNum; ++i)
		{
			resultColor += calculateDirectionalLight(directionalLight[i], material) * vec3(objectColor);
		}

		for (int i = 0; i < pointLightNum; ++i)
		{
			resultColor += calculatePointLight(pointLight[i], material) * vec3(objectColor);
		}

		for (int i = 0; i < spotLightNum; ++i)
		{
			resultColor += calculateSpotLight(spotLight[i], material) * vec3(objectColor);
		}
	}

	if (material.selected)
	{
		FragColor = mix(vec4(resultColor, 1.0), material.selectionColor, 0.5);
	}
	else
	{
		FragColor = vec4(resultColor, 1.0);	
	}
}

vec3 calculateDirectionalLight(DirectionalLight inLight, Material inMaterial)
{
	vec3 ambientColor = inLight.light.ambient * vec3(texture(textureSampler, texPos));

	// vec3 lightDir = normalize(light.position - fragPos);
	vec3 lightDir = normalize(-inLight.direction);

	float diffuseValue = max(dot(fragNormal, lightDir), 0.0);
	vec3 diffuseColor = inLight.light.diffuse * (diffuseValue * inMaterial.diffuse);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormal);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));
	vec3 specularColor = inLight.light.specular * (specularValue * inMaterial.specular);

	return ambientColor + diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight inLight, Material inMaterial)
{
	vec3 lightDir = normalize(inLight.position - fragPos);
	float diffuseValue = max(dot(fragNormal, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormal);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));

	// Attenuation
	float dist = length(inLight.position - fragPos);
	float attenuation = 1.0 / (inLight.constant + inLight.linear * dist + inLight.quadratic * (dist * dist));

	vec3 ambientColor = inLight.light.ambient * vec3(texture(textureSampler, texPos));
	vec3 diffuseColor = inLight.light.diffuse * (diffuseValue * inMaterial.diffuse);
	vec3 specularColor = inLight.light.specular * (specularValue * inMaterial.specular);

	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specularColor *= attenuation;

	return ambientColor + diffuseColor + specularColor;
}

vec3 calculateSpotLight(SpotLight inLight, Material inMaterial)
{
	vec3 lightDir = normalize(inLight.position - fragPos);
	float diffuseValue = max(dot(fragNormal, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, fragNormal);
	float specularValue = pow(max(dot(viewDir, reflectionDir), 0.0), int(inMaterial.shininess));

	// Attenuation
	float dist = length(inLight.position - fragPos);
	float attenuation = 1.0 / (inLight.constant + inLight.linear * dist + inLight.quadratic * (dist * dist));

	float theta = dot(lightDir, normalize(-inLight.direction));
	float epsilon = inLight.cutOff - inLight.outerCutOff;
	float intensity = clamp((theta - inLight.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambientColor = inLight.light.ambient * vec3(texture(textureSampler, texPos));
	vec3 diffuseColor = inLight.light.diffuse * (diffuseValue * inMaterial.diffuse);
	vec3 specularColor = inLight.light.specular * (specularValue * inMaterial.specular);

	ambientColor *= attenuation * intensity;
	diffuseColor *= attenuation * intensity;
	specularColor *= attenuation * intensity;

	return ambientColor + diffuseColor + specularColor;
}
