#include "DynamicCamera.hpp"

DynamicCamera::DynamicCamera(char* name, MoveableEntity* parent, bool useHeading, float headingOffset,
	const glm::vec3& eyeOffset, const glm::vec3& atOffset, const glm::vec3& up, float FOVY,
	float nearClip, float farClip) : StaticCamera(name, parent->Position() + eyeOffset, parent->Position() + atOffset,
		up, FOVY, nearClip, farClip),
	m_pParent(parent),
	m_bUsesHeading(useHeading),
	m_fHeadingOffset(headingOffset),
	m_vEyeOffset(eyeOffset),
	m_vAtOffset(atOffset)
{
	m_vEye = m_fHeadingOffset * m_pParent->Heading() + m_pParent->Position() + m_vEyeOffset;
	m_vAt = m_pParent->Position() + m_vAtOffset;
	m_mViewMatrix = glm::lookAt(m_vEye, m_vAt, m_vUp);
	Scene::Instance()->AddDynamicCamera(this);
}

void DynamicCamera::Update()
{
	// If camera is attached then update
	if (m_pParent != NULL)
	{
		if (m_bUsesHeading) // Uses parent's heading vector
		{
			m_vEye = m_fHeadingOffset * m_pParent->Heading() + m_pParent->Position() + m_vEyeOffset;
		}
		else
		{
			m_vEye = m_pParent->Position() + m_vEyeOffset;
		}

		m_vAt = m_pParent->Position() + m_vAtOffset;
		m_mViewMatrix = glm::lookAt(m_vEye, m_vAt, m_vUp);
	}
}
