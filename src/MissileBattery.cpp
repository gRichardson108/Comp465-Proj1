#include "MissileBattery.hpp"

MissileBattery::MissileBattery(Model* model, CelestialBody* parent, const glm::vec3& pos, const glm::vec3& scale,
	const glm::vec3& target, const glm::vec3& up) :
	MoveableEntity(model, pos, scale, target, up),
	m_pParent(parent)
{
	m_iNumMissiles = 5;
	m_pTargets = new std::vector<MoveableEntity*>();

	// Update position using parent
	if (m_pParent != NULL)
	{
		m_vParentOldPosition = m_pParent->Position();
		m_vPosition += m_vParentOldPosition;
	}

	CreateObjectMatrix();
}

bool MissileBattery::HandleMsg(const Message& message)
{
	return false;
}

void MissileBattery::Update()
{
	if (m_pParent != NULL)
	{
		// Move to origin
		m_vPosition = m_vPosition - m_vParentOldPosition;
		m_vParentOldPosition = m_pParent->Position();

		// Rotate position and axis
		if (m_pParent->RotationRate() > 0.0f)
		{
			m_vPosition = m_pParent->AxisRotation() * m_vPosition;
			m_vForward = glm::normalize(m_pParent->AxisRotation() * m_vForward);
			m_vLeft = glm::normalize(m_pParent->AxisRotation() * m_vLeft);
			m_vUp = glm::normalize(m_pParent->AxisRotation() * m_vUp);
		}

		// Move back to parent;
		m_vPosition += m_vParentOldPosition;

		// Create new matrices
		RotateToForward();
		CreateObjectMatrix();
	}

	// Attempt to launch missile
	if (!m_pActiveMissile && m_iNumMissiles > 0)
	{
		for (std::vector<MoveableEntity*>::iterator it = m_pTargets->begin(); it != m_pTargets->end(); it++)
		{
			if (glm::distance(m_vPosition, (*it)->Position()) <= 5000.0f && 
				glm::dot(m_vForward, (*it)->Position() - m_vPosition) > 0.0f)
			{
				FireMissile();
				break;
			}
		}
	}
}

void MissileBattery::FireMissile()
{
	m_pActiveMissile = new Missile(Scene::Instance()->GetModel("Missile"), m_vPosition, glm::vec3(100.0f), 
		m_vPosition + m_vForward);
	m_pActiveMissile->SetTargets(m_pTargets);
	m_iNumMissiles--;
}
