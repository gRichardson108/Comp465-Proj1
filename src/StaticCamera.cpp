#include "StaticCamera.hpp"

StaticCamera::StaticCamera(char* name, glm::mat4 cameraMatrix, float FOVY, float nearClip, float farClip) :
	m_mViewMatrix(cameraMatrix), m_fFOVY(FOVY), m_fNearClip(nearClip), m_fFarClip(farClip)
{
	size_t size = strlen(name) + 1;
	m_cName = new char[size];
	strncpy(m_cName, name, size);
}

StaticCamera::StaticCamera(char* name, glm::vec3 eye, glm::vec3 at, glm::vec3 up, float FOVY, float nearClip,
	float farClip) : StaticCamera(name, glm::lookAt(eye, at, up), FOVY, nearClip, farClip)
{
	m_vEye = eye;
	m_vAt = at;
	m_vUp = up;
}

glm::mat4 StaticCamera::updateProjectionMatrix(int width, int height)
{
    float aspectRatio = (float) width / (float) height;
    m_mProjectionMatrix = glm::perspective(m_fFOVY, aspectRatio, m_fNearClip, m_fFarClip); 
    return m_mProjectionMatrix;
}
