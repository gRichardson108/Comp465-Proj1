/*
Kory Dondzila
Garret Richardson

DynamicCamera.hpp
10/08/2016

Used for moving cameras in the scene, can be attached
to any MoveableEntity.

*/

#ifndef DYNAMIC_CAMERA_H
#define DYNAMIC_CAMERA_H

#include "StaticCamera.hpp"
#include "MoveableEntity.hpp"

class DynamicCamera : public StaticCamera
{
private :
	MoveableEntity* m_pParent; // Moving entity camera is attached to
	glm::vec3 m_vEyeOffset; // Postion offset from parent
	glm::vec3 m_vAtOffset; // Target offset from parent
	bool m_bUsesHeading; // If Camera uses the heading vector of parent
	float m_fHeadingOffset; // Heading vector offset

public :
	DynamicCamera(char* name, const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float FOVY = glm::radians(60.0f),
		float nearClip = 1.0f, float farClip = 10000000.0f) : 
		StaticCamera(name, eye, at, up, FOVY, nearClip, farClip)
	{
		Scene::Instance()->AddToDynamicQueue(this);
	}

	DynamicCamera(char* name, MoveableEntity* parent, bool useHeading = false, float headingOffset = 0.0f,
		const glm::vec3& eyeOffset = glm::vec3(0.0f), const glm::vec3& atOffset = glm::vec3(0.0f),
		const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float FOVY = glm::radians(60.0f),
		float nearClip = 1.0f, float farClip = 10000000.0f);

	~DynamicCamera() {}
	
	void Update();
	bool UsesHeading() { return m_bUsesHeading; }
	
	float HeadingOffset() { return m_fHeadingOffset; }
	void SetHeadingOffset(float offset) { m_fHeadingOffset = offset; }
};

#endif
