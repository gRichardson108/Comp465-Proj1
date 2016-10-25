#include "Missile.hpp"

Missile::Missile(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up)
{
	m_iLifeTime = m_iTotalLifeTime;
	m_vHeading = m_vForward * m_fVelocity;
}

void Missile::Update()
{
	if (m_bLive)
	{
		if (m_pCurrentTarget != NULL)
		{
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