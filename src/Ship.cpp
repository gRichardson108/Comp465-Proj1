#include "Ship.hpp"

Ship::Ship(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up)
{}

void Ship::keypress(unsigned char key)
{
}

void Ship::rotateYaw(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vUp));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vLeft = glm::normalize(rot * m_vLeft);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::rotatePitch(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vLeft));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vUp = glm::normalize(rot * m_vUp);
    RotateToForward();
    SetHeading(m_vForward);
}

void Ship::rotateRoll(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vForward));
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
            movementRate = currentMaxSpeed;
            break;
        case Msg_KeypressDownArrow:
            movementRate = -currentMaxSpeed;
            break;
        case Msg_KeypressLeftArrow:
            rotateYaw(TURN_RATE);
            break;
        case Msg_KeypressRightArrow:
            rotateYaw(-TURN_RATE);
            break;
        case Msg_ShipPitchDown:
            rotatePitch(TURN_RATE);
            break;
        case Msg_ShipPitchUp:
            rotatePitch(-TURN_RATE);
            break;
        case Msg_ShipRollLeft:
            rotateRoll(-TURN_RATE);
            break;
        case Msg_ShipRollRight:
            rotateRoll(TURN_RATE);
            break;
        case Msg_ShipSpeedChange:
            nextShipSpeed();
            break;
        case Msg_KeyreleaseUpArrow:
        case Msg_KeyreleaseDownArrow:
            movementRate = 0;
        default:

            break;
	}

	return hasMsg;
}
