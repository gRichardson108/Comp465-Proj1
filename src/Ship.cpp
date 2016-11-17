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
    SetHeading();
}

void Ship::rotatePitch(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vLeft));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vUp = glm::normalize(rot * m_vUp);
    RotateToForward();
    SetHeading();
}

void Ship::rotateRoll(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vForward));
    m_vLeft= glm::normalize(rot * m_vLeft);
    m_vUp = glm::normalize(rot * m_vUp);
    RotateToForward();
    SetHeading();
}

void Ship::SetHeading()
{
    m_vHeading = m_vForward * (thrust * currentMaxSpeed) + gravityVector();
}

void Ship::Update()
{
    //thrust variable allows speed changes while thrusting to take effect without releasing the arrow key
    SetHeading();
    m_vPosition = m_vPosition + m_vHeading;
    if (pitchRotation != 0)
    {
        rotatePitch(TURN_RATE * pitchRotation);
    }
    if (yawRotation != 0)
    {
        rotateYaw(TURN_RATE * yawRotation);
    }
    if (rollRotation != 0)
    {
        rotateRoll(TURN_RATE * rollRotation);
    }
    CreateObjectMatrix();
}

bool Ship::HandleMsg(const Message& message)
{
    bool hasMsg = true;
    switch (message.Msg)
    {
        case Msg_CtrlMod_SpecialKeyPress:
            switch (*static_cast<int*>(message.ExtraInfo)){
                case GLUT_KEY_UP:
                    pitchRotation = 1;
                    break;
                case GLUT_KEY_DOWN:
                    pitchRotation = -1;
                    break;
                case GLUT_KEY_LEFT:
                    rollRotation = -1;
                    break;
                case GLUT_KEY_RIGHT:
                    rollRotation = 1;
                    break;
            }
            break;
        case Msg_SpecialKeyPress:
            switch (*static_cast<int*>(message.ExtraInfo)){
                case GLUT_KEY_UP:
                    thrust = 1;
                    break;
                case GLUT_KEY_DOWN:
                    thrust = -1;
                    break;
                case GLUT_KEY_LEFT:
                    yawRotation = 1;
                    break;
                case GLUT_KEY_RIGHT:
                    yawRotation = -1;
                    break;
            }
            break;
        case Msg_SpecialKeyRelease:
            switch (*static_cast<int*>(message.ExtraInfo)){
                case GLUT_KEY_UP:
                case GLUT_KEY_DOWN:
                    thrust = 0;
                    break;
                case GLUT_KEY_LEFT:
                case GLUT_KEY_RIGHT:
                    yawRotation = 0;
                    break;
            }
        case Msg_CtrlMod_SpecialKeyRelease:
            switch (*static_cast<int*>(message.ExtraInfo)){
                case GLUT_KEY_UP:
                case GLUT_KEY_DOWN:
                    pitchRotation = 0;
                    break;
                case GLUT_KEY_LEFT:
                case GLUT_KEY_RIGHT:
                    rollRotation = 0;
                    break;
            }
            break;
        case Msg_ShipSpeedChange:
            nextShipSpeed();
            break;
        default:
            hasMsg = false;
            break;

    }
    return hasMsg;
}

void Ship::nextShipSpeed()
{
    switch (speedSetting)
    {
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

glm::vec3 Ship::gravityVector()
{
    float distance = glm::length(m_vPosition);
    glm::vec3 gravityVec = glm::normalize(m_vPosition * -1.0f) * (GRAVITY / (distance * distance));
    return gravityVec;
}
