// fragment shader
# version 330 core
in vec3 TexCoords;
uniform samplerCube SkyBoxTexture;
out vec4 FragColor;
void main()
{
    FragColor = texture(SkyBoxTexture, TexCoords);
}
