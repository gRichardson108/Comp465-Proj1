#include "Missile.hpp"
#include "DynamicCamera.hpp"

Missile::Missile(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, 
	const glm::vec3& up, float velocity) : 
	MoveableEntity(model, pos, scale, target, up),
	m_iLifeTime(m_iTotalLifeTime),
	m_fVelocity(velocity)
{
	m_vHeading = m_vForward * m_fVelocity;
	m_pTargets = NULL;
	m_pCurrentTarget = NULL;
	new DynamicCamera("Missile", this, false, 0.0f, glm::vec3(0.0, 0.0, 400.0f));
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
			else if (distance <= m_fVelocity)
			{
				m_fVelocity = 0;
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

	CreateObjectMatrix();

	m_iLifeTime--;

	if (m_iLifeTime == 0)
	{
		//delete this;
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

	if (dot <= 0.99999f)
	{
		float maxRadians = glm::half_pi<float>() / 90.0f;
		glm::vec3 tempUp = glm::cross(m_vForward, steeringForce);

		if (glm::length(tempUp) != 0.0f)
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

		if (glm::acos(dot) > maxRadians)
		{
			glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), -(glm::acos(dot) - maxRadians), tempUp));
			steeringForce = rot * steeringForce;
		}

		// Rotate to forward vector and get current up vector
		glm::quat rot1 = glm::rotation(m_vForward, steeringForce);
		glm::vec3 adjustedUp = glm::normalize(rot1 * m_vUp);
		glm::quat rot2 = glm::quat();

		dot = glm::dot(m_vUp, tempUp);
		dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

		if (glm::acos(dot) > maxRadians)
		{
			glm::vec3 tempForward = glm::cross(m_vUp, tempUp);
			if (glm::length(tempForward) != 0.0f)
			{
				tempForward = glm::normalize(tempForward);
			}
			else
			{
				tempForward = glm::normalize(glm::vec3(rand(), rand(), rand()));
				while (colinear(tempForward, m_vUp, 0.1f))
				{
					tempForward = glm::normalize(glm::vec3(rand(), rand(), rand()));
				}
				glm::vec3 tempLeft = glm::cross(tempForward, m_vUp);
				tempForward = glm::normalize(glm::cross(m_vUp, tempLeft));
			}

			glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), -(glm::acos(dot) - maxRadians), tempForward));
			tempUp = rot * tempUp;
		}

		// Get change between current up and object up
		dot = glm::dot(adjustedUp, tempUp);
		dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

		// Get cross which determines direction to rotate
		glm::vec3 cross = glm::cross(adjustedUp, tempUp);
		if (glm::length(cross) != 0.0f)
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
		RotateToForward();
	}

	m_vHeading = m_vForward * m_fVelocity;
	m_vPosition += m_vHeading;
}
