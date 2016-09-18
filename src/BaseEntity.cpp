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

BaseEntity::BaseEntity(Model* model, glm::vec3 pos, glm::vec3 scale, glm::vec3 target) :
	m_vPosition(pos),
	m_mModel(model)
{
	SetScale(scale);
	m_vForward = glm::normalize(target - pos);
	m_vLeft = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_vForward);
	m_vUp = glm::cross(m_vForward, m_vLeft);
}

glm::mat4 BaseEntity::RotateToForward()
{
	glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, -1.0f), m_vForward);
	glm::quat rot2 = glm::rotation(rot1 * glm::vec3(0.0f, 1.0f, 0.0f), m_vUp);
	return glm::mat4_cast(rot2 * rot1);
}
