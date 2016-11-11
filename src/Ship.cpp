#include "Ship.hpp"

Ship::Ship(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up)
{}

void Ship::keypress(unsigned char key)
{
}

void Ship::rotateYaw(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (rotationRate * 1000.0f), m_vUp));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vLeft = glm::normalize(rot * m_vLeft);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::rotatePitch(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (rotationRate * 1000.0f), m_vLeft));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vUp = glm::normalize(rot * m_vUp);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::rotateRoll(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), 5 * glm::two_pi<float>() / (rotationRate * 1000.0f), m_vForward));
    m_vLeft= glm::normalize(rot * m_vLeft);
    m_vUp = glm::normalize(rot * m_vUp);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::Update()
{
    m_vPosition = m_vPosition + (m_vForward * movementRate);
    CreateObjectMatrix();
}

bool Ship::HandleMsg(const Message& message){
    bool hasMsg = false;

	switch (message.Msg)
	{
        case Msg_KeypressUpArrow:
            movementRate = 8.0;
            break;
        case Msg_KeypressDownArrow:
            movementRate = -8.0;
            break;
        case Msg_KeypressLeftArrow:
            rotateYaw(1.0);
            break;
        case Msg_KeypressRightArrow:
            rotateYaw(-1.0);
            break;
        case Msg_ShipPitchDown:
            rotatePitch(1.0);
            break;
        case Msg_ShipPitchUp:
            rotatePitch(-1.0);
            break;
        case Msg_ShipRollLeft:
            rotateRoll(-1.0);
            break;
        case Msg_ShipRollRight:
            rotateRoll(1.0);
            break;
        case Msg_KeyreleaseUpArrow:
        case Msg_KeyreleaseDownArrow:
            movementRate = 0;
        default:

            break;
	}

	return hasMsg;
}
