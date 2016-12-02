// vertex shader
#version 330 core
in vec3 Position;
uniform mat4 MVP;
out vec3 TexCoords;
void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
    TexCoords = Position;
}
