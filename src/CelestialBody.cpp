#include "CelestialBody.h"


CelestialBody::setDefaultRotation()
{
    selfRotationRate = 0.2f;
    rotationAxis = m_vUp;
}

CelestialBody::rotate()
{
    m_vForward = glm::rotate(m_vForward, selfRotationRate, rotationAxis);
    return glm::orientation(m_vForward, m_vUp);
}

CelestialBody::update()
{
    rotationMatrix = rotate();
}
