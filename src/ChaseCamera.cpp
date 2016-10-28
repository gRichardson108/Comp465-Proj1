#include "ChaseCamera.hpp"

ChaseCamera::ChaseCamera(char* name, MoveableEntity* m_pParent, float headingOffset,
            const glm::vec3& eyeOffset, const glm::vec3& atOffset) : DynamicCamera(name, m_pParent, false, headingOffset, eyeOffset, atOffset, m_pParent->Up())
{

}

void ChaseCamera::Update()
{
	// If camera is attached then update
	if (m_pParent != NULL)
	{
        m_vEye = glm::vec3(m_pParent->RotationMatrix() * glm::vec4(m_pParent->Position() + m_vEyeOffset, 0.0));
		m_vAt = glm::vec3(m_pParent->RotationMatrix()*glm::vec4(m_pParent->Position() + m_vAtOffset, 0.0));
		m_mViewMatrix = glm::lookAt(m_vEye, m_vAt, m_vUp);
	}
}

ChaseCamera::~ChaseCamera()
{
    //dtor
}
