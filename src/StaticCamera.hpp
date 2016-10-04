#ifndef STATIC_CAMERA_H
#define STATIC_CAMERA_H

#include "../includes465/include465.hpp"

class StaticCamera {
    private :
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        //viewport width and height for determining aspect ratio
        //we set a default, but this will be wiped out by the reshapeFunc
        int viewportWidth = 800;
        int viewportHeight = 600;
        float FOVY;




    public :
        float nearClip;
        float farClip;
        StaticCamera(glm::mat4 cameraMatrix, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);
        StaticCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);

        glm::mat4 getViewMatrix(void);//setup viewMatrix as read-only

        glm::mat4 updateProjectionMatrix(int width, int height);

        void setFOVY(float newFOVY);
        float getFOVY();
};


#endif
