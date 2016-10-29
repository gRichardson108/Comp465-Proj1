#include "BaseEntity.hpp"

BaseEntity::BaseEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) :
	m_vPosition(pos),
	m_pModel(model)
{
	SetScale(scale);
	m_vForward = glm::normalize(target - pos);
	m_vLeft = glm::normalize(glm::cross(up, m_vForward));
	m_vUp = glm::normalize(glm::cross(m_vForward, m_vLeft));
	RotateToForward();
	CreateObjectMatrix();
	Scene::Instance()->AddEntity(this);
}

void BaseEntity::RotateToForward()
{
	// Rotate to forward vector and get current up vector
	glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, -1.0f), m_vForward);
	glm::vec3 adjustedUp = glm::normalize(rot1 * glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rot2 = glm::quat();

	// Get change between current up and object up
	float dot = glm::dot(adjustedUp, m_vUp);
	dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

	// Get cross which determines direction to rotate
	glm::vec3 cross = glm::cross(adjustedUp, m_vUp);
	if (cross.length() != 0.0f)
	{
		cross = glm::normalize(cross);
	}

	// Rotate around forward vector so current up will
	// face object up
	if (checkSigns(m_vForward, cross))
	{
		rot2 = glm::rotate(rot2, glm::acos(dot), m_vForward);
	}
	else
	{
		rot2 = glm::rotate(rot2, -glm::acos(dot), m_vForward);
	}

	m_mRotation = glm::mat4_cast(rot2 * rot1);
}

void BaseEntity::CreateObjectMatrix()
{
	m_mObject = glm::translate(glm::mat4(), m_vPosition) * m_mRotation * glm::scale(glm::mat4(), m_vScale);
}
