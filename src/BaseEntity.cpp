#include "BaseEntity.hpp"

BaseEntity::BaseEntity(Model* model, glm::vec3 pos, glm::vec3 scale) :
	m_vPosition(pos),
	m_vForward(glm::vec3(0.0, 0.0, -1.0)),
	m_vLeft(glm::vec3(-1.0, 0.0, 0.0)),
	m_vUp(glm::vec3(0.0, 1.0, 0.0)),
	m_mModel(model)
{
	SetScale(scale);
}

BaseEntity::BaseEntity(Model* model, glm::vec3 pos, glm::vec3 scale, glm::vec3 target, glm::vec3 up) :
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
	glm::vec3 adjustedUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = glm::quat();
	float dot = glm::dot(m_vUp, adjustedUp);

	// Adjusted up and desired up might be opposite
	// in this case there's no guarantee which axis the
	// second rotation will be applied to, we want to
	// rotate around the forward axis.
	if (dot >= -1.000001f && dot <= -0.999999f)
	{
		rot2 = glm::rotate(rot2, glm::pi<float>(), m_vForward);
	}
	else
	{
		rot2 = glm::rotation(rot1 * glm::vec3(0.0f, 1.0f, 0.0f), m_vUp);
	}

	return glm::mat4_cast(rot2 * rot1);
}

glm::mat4 BaseEntity::ModelMatrix()
{
	return glm::translate(glm::mat4(), m_vPosition) * RotateToForward() * glm::scale(glm::mat4(), m_vScale);
}
