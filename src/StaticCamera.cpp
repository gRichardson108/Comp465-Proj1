#include "StaticCamera.hpp"

StaticCamera::StaticCamera(glm::mat4 cameraMatrix, float fieldOfViewY, float near, float far):viewMatrix(cameraMatrix),FOVY(fieldOfViewY),nearClip(near),farClip(far){
}

StaticCamera::StaticCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float fieldOfViewY, float near, float far):viewMatrix(glm::lookAt(eye,at,up)),FOVY(fieldOfViewY),nearClip(near),farClip(far){
}

glm::mat4 StaticCamera::getViewMatrix(){
    return viewMatrix;
}

//you must set this as the glutReshapeFunc callback function whenever
//the camera becomes the active view
void reshapeFunc(int width, int height){
    viewportWidth = width;
    viewportHeight = height;
    float aspectRatio = (float) width / (float) height;
    glViewport(0, 0, width, height);
    projectionMatrix = glm::perspective(FOVY, aspectRatio, nearClip, farClip);
}

glm::mat4 updateProjectionMatrix(){
    float aspectRatio = (float) viewportWidth / (float) viewportHeight;
    projectionMatrix = glm::perspective(FOVY, aspectRatio, nearClip, farClip); 
    return projectionMatrix;
}
