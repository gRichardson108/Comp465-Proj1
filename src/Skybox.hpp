#ifndef SKYBOX_H
#define SKYBOX_H

#include "../includes465/include465.hpp"
#include "../includes465/texture.hpp"
#include <string>
#include <vector>


GLuint loadSkyboxTexture(std::vector<const GLchar*> faces);
void loadCubeMapData(const GLchar* filename, int width, int height, GLenum target);

#endif // SKYBOX_H
