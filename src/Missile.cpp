#include "Missile.hpp"

Missile::Missile(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f), 
	const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) : 
	MoveableEntity(model, pos, scale, target, up),
	m_iLifeTime(m_iTotalLifeTime)
{
	m_vHeading = m_vForward * m_fVelocity;
	m_iNumTargets = 0;
	m_pTargets = NULL;
	m_pCurrentTarget = NULL;
}

void Missile::Update()
{
	if (m_bLive)
	{
		if (m_pCurrentTarget != NULL)
		{
			// Call Missile movement

			float distance = glm::distance(m_vPosition, m_pCurrentTarget->Position());
			if (distance > 5000.0f)
			{
				m_pCurrentTarget = NULL;
			}
		}
		else
		{
			float targetDistance = -1.0f;
			for (int i = 0; i < m_iNumTargets; i++)
			{
				float distance = glm::distance(m_vPosition, m_pTargets[i]->Position());
				if (distance <= 5000.0f && (distance < targetDistance || targetDistance < 0))
				{
					targetDistance = distance;
					m_pCurrentTarget = m_pTargets[i];
				}
			}

			if (m_pCurrentTarget != NULL)
			{
				// Call Missile movement
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