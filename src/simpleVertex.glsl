/*
textureModelVertex.glsl

Vertex shader for colored and 
textured surfaces.

Mike Barnes
11/10/2015
*/

# version 330 core
        
in vec4 vPosition, vColor;
in vec3 vNormal;
out vec3 vs_normal, vs_worldPos;
out vec4 vs_color;

uniform mat3 NormalMatrix;
uniform mat4 ModelView;
uniform mat4 MVP;

void main(void)
{
	vec4 position = MVP * vPosition;
	gl_Position = position;
	vs_worldPos = (ModelView * vPosition).xyz;
	vs_normal = NormalMatrix * vNormal;
	vs_color = vColor;
}
 