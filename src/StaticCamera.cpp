#include "StaticCamera.hpp"

StaticCamera::StaticCamera(glm::mat4 cameraMatrix, float fieldOfViewY, float near, float far):viewMatrix(cameraMatrix),FOVY(fieldOfViewY),nearClip(near),farClip(far){
}

StaticCamera::StaticCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float fieldOfViewY, float near, float far):viewMatrix(glm::lookAt(eye,at,up)),FOVY(fieldOfViewY),nearClip(near),farClip(far){
}

glm::mat4 StaticCamera::getViewMatrix(){
    return viewMatrix;
}

glm::mat4 StaticCamera::updateProjectionMatrix(int width, int height){
    float aspectRatio = (float) width / (float) height;
    projectionMatrix = glm::perspective(FOVY, aspectRatio, nearClip, farClip); 
    return projectionMatrix;
}

void StaticCamera::setFOVY(float newFOVY){
    FOVY = newFOVY;
}

float StaticCamera::getFOVY(){
    return FOVY;
}

