#ifndef STATIC_CAMERA_H
#define STATIC_CAMERA_H

#include "../includes465/include465.hpp"
#include "MoveableEntity.hpp"

class StaticCamera {
    private :
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        //viewport width and height for determining aspect ratio
        int viewportWidth;
        int viewportHeight;
        float FOVY;

        //the near and far clipping planes
        float nearClip;
        float farClip;



    public :
        Camera(glm::mat4 cameraMatrix);
        Camera(glm::vec3 eye, glm::vec3 up, glm::vec3 at);

        glm::mat4 getViewMatrix(void);//setup viewMatrix as read-only
        glm::mat4 getProjectionMatrix(void);
        
        

}


#endif
