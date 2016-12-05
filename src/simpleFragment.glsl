/*
textureModelFragment.glsl

Fragment shader for colored and 
textured surfaces.

Mike Barnes
11/10/2015
*/

# version 330 core

in vec4 vs_color;
in vec3 vs_normal, vs_worldPos;
out vec4 color;

struct MaterialInfo
{
	float Ka; // Ambient reflectivity
	float Kd; // Diffuse reflectivity
	float Ks; // Specular reflectivity
	float Shininess; // Specular shininess factor
};

uniform MaterialInfo Material;

uniform vec3 ruberLightPos = vec3(0.0f, 0.0f, 0.0f);
uniform vec3 ruberLightIntensity = vec3(0.8f, 0.5f, 0.1f);

uniform vec3 headLightDir = vec3(0.0f, -1.0f, 0.0f);
vec3 headLightIntensity = vec3(0.5f, 0.5f, 0.5f);

uniform vec4 shipLightPos = vec4(5000.0f, 1300.0f, 6000.0f, 1.0f);
uniform vec3 shipLightDir = vec3(0.0f, 0.0f, -1.0f);
uniform vec3 shipLightIntensity = vec3(1.0f, 1.0f, 1.0f);

uniform bool ruberLightOn = true;
uniform bool headLightOn = true;
uniform bool shipLightOn = true;
uniform bool ambientOn = true;
uniform bool debugOn = false;
uniform bool noLighting = false;

vec3 ambientColor = vec3(1.0, 0.0, 0.0);
vec3 diffuseColor = vec3(0.0, 1.0, 0.0);

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional)
{
	float ambient = Material.Ka;
	float diffuse = 0.0f;
	vec3 n, s;
	float specular = 0.0f;
	float attenuation = 1.0f;

	n = normalize(vs_normal);

	if (directional)
	{
		ambient = 0.2f;
		s = normalize(LightPosition);
		diffuse = Material.Kd * max(dot(s,n), 0.0);
	}
	else
	{
		ambient = 0.0f;
		s = LightPosition - vs_worldPos;
		attenuation = 50 * inversesqrt(length(s));
		s = normalize(s);
		vec3 v = normalize(-vs_worldPos);
		vec3 h = normalize(v + s);

		diffuse = Material.Kd * max(dot(s,n), 0.0); 
		specular = Material.Ks * pow(max(dot(h,n), 0.0), Material.Shininess);
	}

	if (!ambientOn)
		ambient = 0.0f;

	if (debugOn)
	{
		return attenuation * (ambient * ambientColor + diffuse * diffuseColor + specular * vec3(1.0));
	}
	else
	{
		return (ambient + diffuse + specular) * LightIntensity * attenuation;
	}
}

vec3 spotLightFactor(vec3 spotPosition, vec3 spotDirection, vec3 spotIntensity, float spotLightCutoff)
{
	float ambient = Material.Ka;
	if (!ambientOn)
		ambient = 0.0f;

	vec3 n = normalize(vs_normal);
	vec3 s = spotPosition - vs_worldPos;
	float attenuation = 50 * inversesqrt(length(s));
	s = normalize(s);
	vec3 spotDir = normalize(spotDirection);
	float angle = acos(dot(-s, spotDir));
	float cutoff = radians(clamp(spotLightCutoff, 0.0, 90.0));
	float angleAttenuation = 1.0f - angle / cutoff;

	if (angle < cutoff)
	{
		float factor = dot(-s, spotDir) * attenuation * angleAttenuation;
		vec3 v = normalize(-vs_worldPos);
		vec3 h = normalize(v + s);

		if (debugOn)
		{
			return ambient * ambientColor + factor * diffuseColor * Material.Kd * max(dot(s,n), 0.0) + 
				factor * vec3(1.0f) * Material.Ks * pow(max(dot(h,n), 0.0), Material.Shininess) ;
		}
		else
		{
			return ambient * spotIntensity + factor * spotIntensity * (
				Material.Kd * max(dot(s,n), 0.0) + 
				Material.Ks * pow(max(dot(h,n), 0.0), Material.Shininess) );
		}
	}

	if (debugOn)
	{
		return ambient * ambientColor;
	}
	else
	{
		return ambient * spotIntensity;
	}
}

void main(void)
{
	if (!noLighting)
	{
		headLightIntensity = vec3(vs_color) * 0.2f;
		vec3 tempColor = vec3(vs_color) * 0.1f;

		if (ruberLightOn)
			tempColor += vLight(ruberLightPos, ruberLightIntensity, false);

		if (headLightOn)
			tempColor += vLight(headLightDir, headLightIntensity, true);

		if (shipLightOn)
			tempColor += spotLightFactor(vec3(shipLightPos), shipLightDir, shipLightIntensity, 5.0f);
		
		color = vec4(clamp(tempColor, vec3(0.0f), vec3(1.0f)), 1.0f); 
	}
	else
	{
		color = vs_color;
	}
}
