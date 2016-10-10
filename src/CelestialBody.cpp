#include "CelestialBody.hpp"
#include "Scene.hpp"

CelestialBody::CelestialBody(Model* model, CelestialBody* parent, const glm::vec3& pos,
	const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up, float rotationRate,
	float orbitRate, const glm::vec3& orbitAxis) :
	MoveableEntity(model, pos, scale, target, up),
	m_pParent(parent),
	m_fRotationRate(rotationRate),
	m_fOrbitRate(orbitRate),
	m_vOrbitAxis(orbitAxis)
{
	// Get proper orbital axis
	if (m_vPosition != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		m_vHeading = glm::normalize(glm::cross(m_vOrbitAxis, m_vPosition));
		m_vOrbitAxis = glm::normalize(glm::cross(m_vPosition, m_vHeading));
	}

	// Update position using parent
	if (m_pParent != NULL)
	{
		m_vParentOldPosition = m_pParent->Position();
		m_vPosition += m_vParentOldPosition;
		m_fOrbitDistance = (m_vPosition - m_vParentOldPosition).length();
	}
	else
	{
		m_fOrbitDistance = m_vPosition.length();
	}

	CreateObjectMatrix();
	
	// Rates are in seconds for full rotation/orbit
	// multiplied by 40 because that's timeDelay
	m_mAxisRotation = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (m_fRotationRate * 1000.0f), m_vUp));
	m_mOrbit = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (m_fOrbitRate * 1000.0f), m_vOrbitAxis));
}

void CelestialBody::SetPosition(const glm::vec3& position)
{
	m_vPosition = position;

	// Get new orbital axis
	if (m_vPosition != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		m_vHeading = glm::normalize(glm::cross(m_vOrbitAxis, m_vPosition));
		m_vOrbitAxis = glm::normalize(glm::cross(m_vPosition, m_vHeading));
	}

	// New orbit matrix
	m_mOrbit = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (m_fOrbitRate * 1000.0f), m_vOrbitAxis));

	// New position using parent
	if (m_pParent != NULL)
	{
		m_vParentOldPosition = m_pParent->Position();
		m_vPosition += m_vParentOldPosition;
		m_fOrbitDistance = (m_vPosition - m_vParentOldPosition).length();
	}
	else
	{
		m_fOrbitDistance = m_vPosition.length();
	}

	CreateObjectMatrix();
}

void CelestialBody::Update()
{
	// Rotate forward and left vectors
	if (m_fRotationRate > 0.0f)
	{
		m_vForward = glm::normalize(m_mAxisRotation * m_vForward);
		m_vLeft = glm::normalize(m_mAxisRotation * m_vLeft);
	}

	RotateToForward();

	// Old position is due to the parent being
	// updated before child, need to at least remember this
	if (m_pParent != NULL)
	{
		m_vPosition = m_vPosition - m_vParentOldPosition + m_pParent->Position();
		m_vParentOldPosition = m_pParent->Position();
	}

	// Rotate position around origin
	if (m_fOrbitRate > 0.0f)
	{
		if (m_pParent != NULL)
		{
			// Need to remove parent position first
			m_vPosition = m_mOrbit * (m_vPosition - m_vParentOldPosition) + m_vParentOldPosition;
		}
		else
		{
			m_vPosition = m_mOrbit * m_vPosition;
		}

		m_vHeading = glm::normalize(m_mOrbit * m_vHeading);
	}

	CreateObjectMatrix();
}
