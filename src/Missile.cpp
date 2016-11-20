#include "Missile.hpp"
#include "DynamicCamera.hpp"

Missile::Missile(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up, float velocity) :
	MoveableEntity(model, pos, scale, target, up),
	m_iLifeTime(m_iTotalLifeTime),
	m_bDestroyed(false),
	m_bLive(false),
	m_fVelocity(velocity)
{
	m_vHeading = m_vForward * m_fVelocity;
	m_pTargets = new std::vector<MoveableEntity*>();
	m_pCurrentTarget = NULL;
	new DynamicCamera("Missile", this, false, 0.0f, glm::vec3(0.0, 100.0, 400.0f), glm::vec3(0.0, 100.0f, 0.0));
}

bool Missile::HandleMsg(const Message& message)
{
	bool hasMsg = false;

	switch (message.Msg)
	{
		// Destroy source
		case Msg_DestroySource:
			hasMsg = true;
			if (m_pCurrentTarget && message.Sender == m_pCurrentTarget->ID())
			{
				m_pCurrentTarget = NULL;
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
			hasMsg = true;
			m_pCurrentTarget = NULL;
			m_pTargets->clear();
			MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);
			break;

		default:
			break;
	}

	return hasMsg;
}

void Missile::Update()
{
	if (m_bLive)
	{
		if (m_pCurrentTarget)
		{
			// Close to target, turn towards it
			float distance = glm::distance(m_vPosition, m_pCurrentTarget->Position());
			if (distance > 5000.0f)
			{
				m_pCurrentTarget = NULL;
			}
			else
			{
				MissileGuidance();
			}
		}
		else
		{
			// Find closest target and turn to it
			float targetDistance = -1.0f;
			if (m_pTargets->size() > 0)
			{
				for (auto e : *m_pTargets)
				{
					float distance = glm::distance(m_vPosition, e->Position());
					if (distance <= 5000.0f && (distance < targetDistance || targetDistance < 0))
					{
						targetDistance = distance;
						m_pCurrentTarget = e;
					}
				}
			}

			if (m_pCurrentTarget)
			{
				MissileGuidance();
			}
		}
	}

	// Get new heading and move
	m_vHeading = m_vForward * m_fVelocity;
	m_vPosition += m_vHeading;

	if (m_bLive)
	{
		CheckCollisions();
	}

	if (!m_bDestroyed)
	{
		CreateObjectMatrix();
		m_iLifeTime--;

		// Life expired, destroy self
		if (m_iLifeTime == 0)
		{
			MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);
			m_pCurrentTarget = NULL;
			Scene::Instance()->DestroyEntity(m_iID);
		}
		else if (m_iTotalLifeTime - m_iLifeTime >= 200)
		{
			m_bLive = true;
		}
	}
}

void Missile::MissileGuidance()
{
	glm::vec3 steeringForce = glm::normalize(m_pCurrentTarget->Position() - m_vPosition);

	float dot = glm::dot(m_vForward, steeringForce);
	dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

	// Not facing target, need to turn
	if (dot <= 0.99999f)
	{
		float maxRadians = glm::half_pi<float>() / 90.0f;
		glm::vec3 tempUp = glm::cross(m_vForward, steeringForce); // First axis to rotate on

		if (glm::length(tempUp) != 0.0f)
		{
			tempUp = glm::normalize(tempUp);
		}

		if (dot <= -0.99999f) // Facing opposite the direction target is in
		{
			// Turn in any direction
			tempUp = glm::normalize(glm::vec3(rand(), rand(), rand()));
			while (colinear(tempUp, m_vForward, 0.1f))
			{
				tempUp = glm::normalize(glm::vec3(rand(), rand(), rand()));
			}
			glm::vec3 tempLeft = glm::cross(tempUp, m_vForward);
			tempUp = glm::normalize(glm::cross(m_vForward, tempLeft));
		}

		// Restrict rotation amount
		if (glm::acos(dot) > maxRadians)
		{
			glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), -(glm::acos(dot) - maxRadians), tempUp));
			steeringForce = rot * steeringForce;
		}

		// Rotate to steering direction
		glm::quat rot1 = glm::rotation(m_vForward, steeringForce);
		glm::vec3 adjustedUp = glm::normalize(rot1 * m_vUp);
		glm::quat rot2 = glm::quat();

		dot = glm::dot(m_vUp, tempUp);
		dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);

		// quat rotation caused missile to rotate on forward axis more than maxradians
		if (glm::acos(dot) > maxRadians)
		{
			glm::vec3 tempForward = glm::cross(m_vUp, tempUp); // Secondary rotation axis
			if (glm::length(tempForward) != 0.0f)
			{
				tempForward = glm::normalize(tempForward);
			}

			if (dot <= -0.99999f) // Possible bad axis
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

		// Apply rotation to vectors and get new rotation matrix
		glm::mat3 localRotation = glm::mat3_cast(rot2 * rot1);
		m_vForward = localRotation * m_vForward;
		m_vLeft = localRotation * m_vLeft;
		m_vUp = localRotation * m_vUp;
		CreateRotationMatrix();
	}
}

void Missile::CheckCollisions()
{
	for (int id : *Scene::Instance()->CollidableObjects())
	{
		// Skip self
		if (m_iID == id) continue;

		MoveableEntity* obj = (MoveableEntity*)Scene::Instance()->GetEntityFromID(id);
		float boundingOffset = 10;
		std::string type = obj->GetType();

		// Bounding offset only applies to missile and ship
		if ("Ship" != type && "Missile" != type)
		{
			boundingOffset = 0;
		}

		// Object hit?
		float distance = glm::distance(m_vPosition, obj->Position());
		if (distance <= m_fBoundingRadius + 10 + obj->BoundingRadius() + boundingOffset)
		{
			MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);

			// If destructable type, then destroy
			if ("Ship" == type || "Missile" == type || "MissileBattery" == type)
			{
                printf("Missile strikes object of type %s\n", type);
				MessageDispatcher::Instance()->DispatchMsg(0, m_iID, obj->ID(), Msg_TargetDestroyed, NULL);
			}

			m_pCurrentTarget = NULL;
			Scene::Instance()->DestroyEntity(m_iID);
			m_bDestroyed = true;
		}
	}
}
