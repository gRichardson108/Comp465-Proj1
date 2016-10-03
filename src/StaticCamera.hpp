#ifndef STATIC_CAMERA_H
#define STATIC_CAMERA_H

#include "../includes465/include465.hpp"
#include "MoveableEntity.hpp"

class StaticCamera
{
    private :
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        //viewport width and height for determining aspect ratio
        //we set a default, but this will be wiped out by the reshapeFunc
        int viewportWidth = 800;
        int viewportHeight = 600;
        float FOVY;
		float m_fNearClip;
		float m_fFarClip;

    public :
        StaticCamera(glm::mat4 cameraMatrix, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);
        StaticCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float FOVY = glm::radians(60.0f), float nearClip = 1.0f, float farClip = 10000000.0f);

        glm::mat4 getViewMatrix(void);//setup viewMatrix as read-only

        glm::mat4 updateProjectionMatrix(int width, int height);

		float NearClip() { return m_fNearClip; }
		void SetNearClip(float nearClip) { m_fNearClip = nearClip; }
		float FarClip() { return m_fFarClip; }
		void SetFarClip(float farClip) { m_fFarClip = farClip; }

        void setFOVY(float newFOVY);
        float getFOVY();
};

#endif
