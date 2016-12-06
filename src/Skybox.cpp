#include "Skybox.hpp"

GLuint loadSkyboxTexture(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width = 2048;
    int height = 2048;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        loadCubeMapData(faces[i], width, height, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void loadCubeMapData(const GLchar* filename, int width, int height, GLenum target)
{
    unsigned char * data;
    FILE * file;
    int readResult;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL )
    {
        printf("File %s can't be opened\n", filename);
        return;
    }
    // allocate buffer -- 3 bytes / texel -- rgb
    data = (unsigned char *) malloc( width * height * 3 );
    // read texture data
    readResult = fread( data, width * height * 3, 1, file );
    if (readResult != 1)
    {
        printf("File %s was not read correctly\n", filename);
        return;
    }
    fclose( file );
    glTexImage2D(target, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    free( data ); //free the texture
    return;
}
