#ifndef STATIC_CAMERA_H
#define STATIC_CAMERA_H

#include "../includes465/include465.hpp"

class StaticCamera
{
protected :
    glm::mat4 m_mViewMatrix;
    glm::mat4 m_mProjectionMatrix;
	glm::vec3 m_vEye;
	glm::vec3 m_vAt;
	glm::vec3 m_vUp;

    //viewport width and height for determining aspect ratio
    //we set a default, but this will be wiped out by the reshapeFunc
    int m_iViewportWidth = 800; // Viewport width
    int m_iViewportHeight = 600; // Viewport height
    float m_fFOVY; // Field of view
	float m_fNearClip; // Near clipping plane
	float m_fFarClip; // Far clipping plane
	char* m_cName; // Name of camera

public :
    StaticCamera(char* name, glm::mat4 cameraMatrix, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);
    StaticCamera(char* name, glm::vec3 eye, glm::vec3 at, glm::vec3 up, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);

	glm::mat4 ViewMatrix() { return m_mViewMatrix; }

    glm::mat4 updateProjectionMatrix(int width, int height);

	float NearClip() { return m_fNearClip; }
	void SetNearClip(float nearClip) { m_fNearClip = nearClip; }

	float FarClip() { return m_fFarClip; }
	void SetFarClip(float farClip) { m_fFarClip = farClip; }

	float FOVY() { return m_fFOVY; }
	void SetFOVY(float newFOVY) { m_fFOVY = newFOVY; }

	char* Name() { return m_cName; }
};

#endif
