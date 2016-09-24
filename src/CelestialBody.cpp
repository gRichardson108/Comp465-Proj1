#include "CelestialBody.hpp"


void CelestialBody::setDefaultRotation()
{
    selfRotationRate = 0.2f;
    rotationAxis = m_vUp;
}

glm::mat4 CelestialBody::rotate()
{
    return glm::rotate(rotationMatrix, selfRotationRate, rotationAxis);
}

void CelestialBody::update()
{
    rotationMatrix = rotate();
}
