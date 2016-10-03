#include "StaticCamera.hpp"

StaticCamera::StaticCamera(glm::mat4 cameraMatrix, float fieldOfViewY, float nearClip, float farClip) : 
	viewMatrix(cameraMatrix),FOVY(fieldOfViewY),m_fNearClip(nearClip),m_fFarClip(farClip)
{}

StaticCamera::StaticCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float fieldOfViewY, float nearClip, float farClip) :
	viewMatrix(glm::lookAt(eye,at,up)),FOVY(fieldOfViewY),m_fNearClip(nearClip),m_fFarClip(farClip)
{}

glm::mat4 StaticCamera::getViewMatrix()
{
    return viewMatrix;
}

glm::mat4 StaticCamera::updateProjectionMatrix(int width, int height){
    float aspectRatio = (float) width / (float) height;
    projectionMatrix = glm::perspective(FOVY, aspectRatio, m_fNearClip, m_fFarClip); 
    return projectionMatrix;
}

void StaticCamera::setFOVY(float newFOVY){
    FOVY = newFOVY;
}

float StaticCamera::getFOVY(){
    return FOVY;
}

