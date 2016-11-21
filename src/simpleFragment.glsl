/*
textureModelFragment.glsl

Fragment shader for colored and 
textured surfaces.

Mike Barnes
11/10/2015
*/

# version 330 core
        
in vec2 vs_texCoord;
in vec4 vs_color;
in vec3 vs_normal, vs_worldPos;
out vec4 color;

uniform bool IsTexture; 
uniform sampler2D Texture;
uniform vec3 ruberLightPos = vec3(0.0f, 0.0f, 0.0f);
uniform vec3 ruberLightIntensity = vec3(0.8f, 0.5f, 0.1f);
uniform vec3 headLightDir = vec3(0.0f, -1.0f, 0.0f);
vec3 headLightIntensity = vec3(0.5f, 0.5f, 0.5f);
uniform vec3 shipLight = vec3(5000.0f, 1300.0f, 6000.0f);

uniform bool ruberLightOn = true;
uniform bool headLightOn = true;
uniform bool shipLightOn = false;
uniform bool ambientOn = true;
uniform bool debugOn = false;
uniform bool noLighting = false;

vec3 ambientColor = vec3(1.0, 0.0, 0.0);
vec3 diffuseColor = vec3(0.0, 1.0, 0.0);

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional)
{
	float ambient = 0.2f;
	if (!ambientOn)
		ambient = 0.0f;
	float diffuse = 0.0f;
	vec3 n, s;
	if (directional)
		s = normalize(LightPosition);
	else
	{
		s = normalize(LightPosition - vs_worldPos);
		ambient = 0.0f;
	}

	n = normalize(vs_normal);
	diffuse = max(dot(s, n), 0.0);
	if (debugOn)
	{
		return ambient * ambientColor + diffuse * diffuseColor;
	}
	else
	{
		return (ambient + diffuse) * LightIntensity;
	}
}

void main(void)
{
	if (!noLighting)
	{
		headLightIntensity = vec3(vs_color) * 0.3f;
		vec3 tempColor = vec3(vs_color) * 0.2f;
		if (ruberLightOn)
			tempColor += vLight(ruberLightPos, ruberLightIntensity, false);
		if (headLightOn)
			tempColor += vLight(headLightDir, headLightIntensity, true);
		
		if (IsTexture)   // use texture on surface
			color = texture(Texture, vs_texCoord); 
		else         // use color for surface
			color = vec4(tempColor, 1.0f); 
	}
	else
	{
		color = vs_color;
	}
}
