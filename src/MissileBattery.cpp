#include "MissileBattery.hpp"

MissileBattery::MissileBattery(Model* model, CelestialBody* parent, const glm::vec3& pos, const glm::vec3& scale,
	const glm::vec3& target, const glm::vec3& up) :
	MoveableEntity(model, pos, scale, target, up),
	m_pParent(parent)
{
	m_iNumMissiles = 5;
	m_pTargets = new std::vector<MoveableEntity*>();
	m_pActiveMissile = NULL;

	// Update position using parent
	if (m_pParent)
	{
		m_vParentOldPosition = m_pParent->Position();
		m_vPosition += m_vParentOldPosition;
	}

	CreateObjectMatrix();
}

bool MissileBattery::HandleMsg(const Message& message)
{
	bool hasMsg = true;

	switch (message.Msg)
	{
		// Remove reference to the sender
		case Msg_DestroySource :
			printf("Msg: Destroy source received by missile battery ID: %d\n", m_iID);
			if (m_pActiveMissile && message.Sender == m_pActiveMissile->ID())
			{
				m_pActiveMissile = NULL;
			}
			else if (m_pParent && message.Sender == m_pParent->ID())
			{
				// Since parent is destroyed so is this
				m_pParent = NULL;
				printf("Msg: Destroy source sent by missile battery ID: %d\n", m_iID);
				MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);
				m_pActiveMissile = NULL;
				m_pTargets->clear();
				Scene::Instance()->DestroyEntity(m_iID);
			}
			else
			{
				if (m_pTargets->size() > 0)
				{
					for (auto it = m_pTargets->begin(); it != m_pTargets->end(); it++)
					{
						if (message.Sender == (*it)->ID())
						{
							m_pTargets->erase(it);
							break;
						}
					}
				}
			}
			break;

		// Destroy self
		case Msg_TargetDestroyed:
			printf("Msg: Destroy target received by missile battery ID: %d\n", m_iID);
			m_pActiveMissile = NULL;
			m_pParent = NULL;
			m_pTargets->clear();
			printf("Msg: Destroy source sent by missile battery ID: %d\n", m_iID);
			MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);
			Scene::Instance()->DestroyEntity(m_iID);
			break;

		default :
			hasMsg = false;
			break;
	}

	return hasMsg;
}

void MissileBattery::Update()
{
	if (m_pParent)
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
		CreateRotationMatrix();
		CreateObjectMatrix();
	}

	// Attempt to launch missile
	if (!m_pActiveMissile && m_iNumMissiles > 0)
	{
		if (m_pTargets->size() > 0)
		{
			for (auto e : *m_pTargets)
			{
				if (glm::distance(m_vPosition, e->Position()) <= 5000.0f &&
					glm::dot(m_vForward, e->Position() - m_vPosition) > 0.0f)
				{
					FireMissile();
					break;
				}
			}
		}
	}
}

void MissileBattery::FireMissile()
{
	printf("Missile launced by missile battery ID: %d\n", m_iID);
	m_pActiveMissile = new Missile(Scene::Instance()->GetModel("Missile"), m_vPosition, glm::vec3(25.0f),
		m_vPosition + m_vForward);
	m_pActiveMissile->SetTargets(m_pTargets);
	m_iNumMissiles--;
}

void MissileBattery::SetTargets(const std::string& type)
{
	for (auto entity : *Scene::Instance()->Entities())
	{
		if (StringICompare(type, entity.second->GetType()))
		{
			AddTarget((MoveableEntity*)entity.second);
		}
	}
}

void MissileBattery::AddTarget(MoveableEntity* target)
{
	m_pTargets->push_back(target);
}

void MissileBattery::RemoveTarget(MoveableEntity* target)
{
	for (auto it = m_pTargets->begin(); it != m_pTargets->end(); it++)
	{
		if (target == *it)
		{
			m_pTargets->erase(it);
			break;
		}
	}
}
