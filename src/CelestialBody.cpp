#include "CelestialBody.hpp"

CelestialBody::CelestialBody(Model* model, CelestialBody* parent, const glm::vec3& pos,
	const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up, float rotationRate,
	float orbitRate, const glm::vec3& orbitAxis) :
	MoveableEntity(model, pos, scale, target, up),
	m_eParent(parent),
	m_fRotationRate(rotationRate),
	m_fOrbitRate(orbitRate),
	m_vOrbitAxis(orbitAxis)
{
	if (m_eParent != NULL)
	{
		m_vPosition += m_eParent->Position();
		m_fOrbitDistance = (m_vPosition - m_eParent->Position()).length();
	}
	else
	{
		m_fOrbitDistance = m_vPosition.length();
	}
	
	// Rates are in seconds for full rotation/orbit
	// multiplied by 40 because that's timeDelay
	m_mRotation = glm::mat3(glm::rotate(glm::mat4(), 40 * glm::two_pi<float>() / (m_fRotationRate * 1000.0f), m_vUp));
	m_mOrbit = glm::mat3(glm::rotate(glm::mat4(), 40 * glm::two_pi<float>() / (m_fOrbitRate * 1000.0f), m_vOrbitAxis));
}

void CelestialBody::SetPosition(const glm::vec3& position)
{
	m_vPosition = position;

	if (m_eParent != NULL)
	{
		m_vParentOldPosition = m_eParent->Position();
		m_vPosition += m_vParentOldPosition;
		m_fOrbitDistance = (m_vPosition - m_vParentOldPosition).length();
	}
	else
	{
		m_fOrbitDistance = m_vPosition.length();
	}
}

void CelestialBody::Update()
{
	if (m_fRotationRate > 0.0f)
	{
		// There seems to be an issue where occasionally there's a quick scale change
		// I think this has something to do with changing these vectors but haven't
		// figured it out yet.
		m_vForward = glm::normalize(m_mRotation * m_vForward);
		m_vLeft = glm::normalize(m_mRotation * m_vLeft);
	}

	// Old position is due to the parent being
	// updated before child, need to at least remember this
	if (m_eParent != NULL)
	{
		m_vPosition = m_vPosition - m_vParentOldPosition + m_eParent->Position();
		m_vParentOldPosition = m_eParent->Position();
	}

	if (m_fOrbitRate > 0.0f)
	{
		if (m_eParent != NULL)
		{
			m_vPosition = m_mOrbit * (m_vPosition - m_vParentOldPosition) + m_vParentOldPosition;
		}
		else
		{
			m_vPosition = m_mOrbit * m_vPosition;
		}
	}
}
