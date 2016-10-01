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
	m_vLeft = glm::normalize(glm::cross(up, m_vForward));
	m_vUp = glm::normalize(glm::cross(m_vForward, m_vLeft));
}

glm::mat4 BaseEntity::RotateToForward()
{
	// Rotate to forward vector and get current up vector
	glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, -1.0f), m_vForward);
	glm::vec3 adjustedUp = glm::normalize(rot1 * glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rot2 = glm::quat();

	// Get change between current up and object up
	float dot = glm::dot(m_vUp, adjustedUp);
	dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

	// Get cross which determines direction to rotate
	glm::vec3 cross = glm::cross(adjustedUp, m_vUp);
	if (cross.length() != 0.0f)
	{
		cross = glm::normalize(cross);
	}

	//printf("Adjust Up: %1.10f\n", dot);
	//showVec3("Forward", m_vForward);
	//showVec3("Cross", cross);

	if (colinear(m_vForward, cross, 0.1))
	{
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
	}
	else
	{
		//  I don't think this is ever called, at least it hasn't for me
		rot2 = glm::rotation(adjustedUp, m_vUp);
	}

	return glm::mat4_cast(rot2 * rot1);
}

glm::mat4 BaseEntity::ModelMatrix()
{
	return glm::translate(glm::mat4(), m_vPosition) * RotateToForward() * glm::scale(glm::mat4(), m_vScale);
}
