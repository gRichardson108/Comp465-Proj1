#include "Ship.hpp"

Ship::Ship(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up)
{}

void Ship::keypress(unsigned char key, int x, int y)
{
}

void Ship::Update()
{
    m_vPosition = m_vPosition + (m_vHeading * movementRate);
    CreateObjectMatrix();
}
