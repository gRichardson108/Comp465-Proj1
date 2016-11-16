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
    //thrust variable allows speed changes while thrusting to take effect without releasing the arrow key
    m_vPosition = m_vPosition + (m_vForward * (thrust * currentMaxSpeed));
    if (pitchRotation != 0){
        rotatePitch(TURN_RATE * pitchRotation);
    }
    if (yawRotation != 0){
        rotateYaw(TURN_RATE * yawRotation);
    }
    if (rollRotation != 0){
        rotateRoll(TURN_RATE * rollRotation);
    }
    CreateObjectMatrix();
}

bool Ship::HandleMsg(const Message& message){
    bool hasMsg = false;

	switch (message.Msg)
	{
        case Msg_KeypressUpArrow:
            thrust = 1;
            break;
        case Msg_KeypressDownArrow:
            thrust = -1;
            break;
        case Msg_KeypressLeftArrow:
            yawRotation = 1;
            break;
        case Msg_KeypressRightArrow:
            yawRotation = -1;
            break;
        case Msg_ShipPitchDown:
            pitchRotation = 1;
            break;
        case Msg_ShipPitchUp:
            pitchRotation = -1;
            break;
        case Msg_ShipRollLeft:
            rollRotation = -1;
            break;
        case Msg_ShipRollRight:
            rollRotation = 1;
            break;
        case Msg_ShipSpeedChange:
            nextShipSpeed();
            break;
        case Msg_KeyreleaseUpArrow:
        case Msg_KeyreleaseDownArrow:
            thrust = 0;
            pitchRotation = 0;
            break;
        case Msg_KeyreleaseLeftArrow:
        case Msg_KeyreleaseRightArrow:
            rollRotation = 0;
            yawRotation = 0;
            break;
        default:

            break;
	}

	return hasMsg;
}

void Ship::nextShipSpeed(){
            switch (speedSetting){
                case SPEED_DEFAULT:
                    speedSetting = SPEED_FAST;
                    currentMaxSpeed = 50;
                    break;
                case SPEED_FAST:
                    speedSetting = SPEED_DEBUG;
                    currentMaxSpeed = 200;
                    break;
                case SPEED_DEBUG:
                    speedSetting = SPEED_DEFAULT;
                    currentMaxSpeed = 10;
                    break;
            }
            printf("speed setting: %f\n", currentMaxSpeed);
        }
