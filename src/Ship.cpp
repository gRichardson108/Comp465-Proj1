#include "Ship.hpp"

Ship::Ship(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up)
{}

void Ship::keypress(unsigned char key, int x, int y)
{
}

void Ship::rotateYaw(float rotationRate)
{
    m_vForward = glm::normalize(glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (rotationRate * 1000.0f), m_vUp)) * m_vForward);
    m_vLeft = glm::normalize(m_vUp * m_vLeft);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::Update()
{
    m_vPosition = m_vPosition + (m_vForward * movementRate);
    CreateObjectMatrix();
}
