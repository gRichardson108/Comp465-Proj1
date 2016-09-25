#include "BaseEntity.hpp"

BaseEntity::BaseEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale) :
	m_vPosition(pos),
	m_vForward(glm::vec3(0.0, 0.0, -1.0)),
	m_vLeft(glm::vec3(-1.0, 0.0, 0.0)),
	m_vUp(glm::vec3(0.0, 1.0, 0.0)),
	m_mModel(model)
{
	SetScale(scale);
}

BaseEntity::BaseEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) :
	m_vPosition(pos),
	m_mModel(model)
{
	SetScale(scale);
	m_vForward = glm::normalize(target - pos);
	m_vLeft = glm::normalize(glm::cross(glm::normalize(up), m_vForward));
	m_vUp = glm::normalize(glm::cross(m_vForward, m_vLeft));
}

glm::mat4 BaseEntity::RotateToForward()
{
	glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, -1.0f), m_vForward);
	glm::vec3 adjustedUp = glm::normalize(rot1 * glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rot2 = glm::quat();
	float dot = glm::dot(m_vUp, adjustedUp);
	glm::vec3 cross = glm::normalize(glm::cross(adjustedUp, m_vUp));

	if (checkSigns(m_vForward, cross))
	{
		rot2 = glm::rotate(rot2, glm::acos(dot), m_vForward);
	}
	else
	{
		rot2 = glm::rotate(rot2, -glm::acos(dot), m_vForward);
	}

	return glm::mat4_cast(rot2 * rot1);
}

glm::mat4 BaseEntity::ModelMatrix()
{
	return glm::translate(glm::mat4(), m_vPosition) * RotateToForward() * glm::scale(glm::mat4(), m_vScale);
}
