#include "Missile.hpp"

Missile::Missile(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, 
	const glm::vec3& up, float velocity) : 
	MoveableEntity(model, pos, scale, target, up),
	m_iLifeTime(m_iTotalLifeTime),
	m_fVelocity(velocity)
{
	m_vHeading = m_vForward * m_fVelocity;
	m_pTargets = NULL;
	m_pCurrentTarget = NULL;
}

void Missile::Update()
{
	if (m_bLive)
	{
		if (m_pCurrentTarget != NULL)
		{
			MissileGuidance();

			float distance = glm::distance(m_vPosition, m_pCurrentTarget->Position());
			if (distance > 5000.0f)
			{
				m_pCurrentTarget = NULL;
			}
		}
		else
		{
			float targetDistance = -1.0f;
			for (std::vector<MoveableEntity*>::iterator it = m_pTargets->begin(); it != m_pTargets->end(); it++)
			{
				float distance = glm::distance(m_vPosition, (*it)->Position());
				if (distance <= 5000.0f && (distance < targetDistance || targetDistance < 0))
				{
					targetDistance = distance;
					m_pCurrentTarget = (*it);
				}
			}

			if (m_pCurrentTarget != NULL)
			{
				MissileGuidance();
			}
			else
			{
				m_vHeading = m_vForward * m_fVelocity;
				m_vPosition += m_vHeading;
			}
		}
	}
	else
	{
		m_vPosition += m_vHeading;
	}

	m_iLifeTime--;

	if (m_iLifeTime == 0)
	{
		delete this;
	}
	else if (m_iTotalLifeTime - m_iLifeTime >= 200)
	{
		m_bLive = true;
	}
}

void Missile::MissileGuidance()
{
	glm::vec3 steeringForce = glm::normalize(m_pCurrentTarget->Position() - m_vPosition);

	float dot = glm::dot(m_vForward, steeringForce);
	dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

	if (glm::acos(dot) > glm::quarter_pi<float>())
	{
		glm::vec3 tempUp = glm::cross(m_vForward, steeringForce);
		if (tempUp.length() != 0.0f)
		{
			tempUp = glm::normalize(tempUp);
		}
		else
		{
			tempUp = glm::normalize(glm::vec3(rand(), rand(), rand()));
			while (colinear(tempUp, m_vForward, 0.1f))
			{
				tempUp = glm::normalize(glm::vec3(rand(), rand(), rand()));
			}
			glm::vec3 tempLeft = glm::cross(tempUp, m_vForward);
			tempUp = glm::normalize(glm::cross(m_vForward, tempLeft));
		}

		glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), -(glm::acos(dot) - glm::quarter_pi<float>()), tempUp));
		steeringForce = rot * steeringForce;
	}

	// Rotate to forward vector and get current up vector
	glm::quat rot1 = glm::rotation(m_vForward, steeringForce);
	glm::vec3 adjustedUp = glm::normalize(rot1 * m_vUp);
	glm::vec3 newUp = glm::normalize(glm::cross(m_vForward, steeringForce));
	glm::quat rot2 = glm::quat();

	// Get change between current up and object up
	dot = glm::dot(adjustedUp, newUp);
	dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

	// Get cross which determines direction to rotate
	glm::vec3 cross = glm::cross(adjustedUp, newUp);
	if (cross.length() != 0.0f)
	{
		cross = glm::normalize(cross);
	}

	// Rotate around forward vector so current up will
	// face object up
	if (checkSigns(steeringForce, cross))
	{
		rot2 = glm::rotate(rot2, glm::acos(dot), steeringForce);
	}
	else
	{
		rot2 = glm::rotate(rot2, -glm::acos(dot), steeringForce);
	}

	glm::mat3 localRotation = glm::mat3_cast(rot2 * rot1);
	m_vForward = localRotation * m_vForward;
	m_vLeft = localRotation * m_vLeft;
	m_vUp = localRotation * m_vUp;
	m_vHeading = m_vForward * m_fVelocity;
	m_vPosition += m_vHeading;
}
