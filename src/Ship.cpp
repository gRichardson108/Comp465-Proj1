#include "Ship.hpp"
#include "DynamicCamera.hpp"
#include "MissileBattery.hpp"
#include "Scene.hpp"

Ship::Ship(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target,
	const glm::vec3& up) :
	MoveableEntity(model, pos, scale, target, up),
	m_iPitchRotation(0),
	m_iRollRotation(0),
	m_iYawRotation(0),
	m_iThrust(0),
	m_iNumMissiles(9),
	m_pTargets(new std::vector<MoveableEntity*>()),
	m_pActiveMissile(NULL)
{}

void Ship::RotateYaw(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vUp));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vLeft = glm::normalize(rot * m_vLeft);
	CreateRotationMatrix();
}

void Ship::RotatePitch(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vLeft));
    m_vForward = glm::normalize(rot * m_vForward);
    m_vUp = glm::normalize(rot * m_vUp);
	CreateRotationMatrix();
}

void Ship::RotateRoll(float rotationRate)
{
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(), rotationRate, m_vForward));
    m_vLeft= glm::normalize(rot * m_vLeft);
    m_vUp = glm::normalize(rot * m_vUp);
    CreateRotationMatrix();
}

void Ship::SetHeading()
{
    m_vHeading = m_vForward * (m_iThrust * currentMaxSpeed);
    if (m_bGravityStatus){
        m_vHeading += GravityVector();
    }
}

void Ship::Update()
{
	if (m_iYawRotation != 0)
	{
		RotateYaw(TURN_RATE * m_iYawRotation);
	}
    if (m_iPitchRotation != 0)
    {
        RotatePitch(TURN_RATE * m_iPitchRotation);
    }
    if (m_iRollRotation != 0)
    {
        RotateRoll(TURN_RATE * m_iRollRotation);
    }

	SetHeading();
    m_vPosition = m_vPosition + m_vHeading;
	CheckCollisions();
    CreateObjectMatrix();
}

bool Ship::HandleMsg(const Message& message)
{
    bool hasMsg = true;
    switch (message.Msg)
    {
        case Msg_CtrlMod_SpecialKeyPress: // Pitch and Roll
            switch (DereferenceToType<int>(message.ExtraInfo))
			{
                case GLUT_KEY_UP:
                    m_iPitchRotation = 1;
                    break;
                case GLUT_KEY_DOWN:
                    m_iPitchRotation = -1;
                    break;
                case GLUT_KEY_LEFT:
					m_iRollRotation = -1;
                    break;
                case GLUT_KEY_RIGHT:
					m_iRollRotation = 1;
                    break;
            }
            break;
        case Msg_SpecialKeyPress: // Move and Yaw
            switch (DereferenceToType<int>(message.ExtraInfo))
			{
                case GLUT_KEY_UP:
					m_iThrust = 1;
                    break;
                case GLUT_KEY_DOWN:
					m_iThrust = -1;
                    break;
                case GLUT_KEY_LEFT:
					m_iYawRotation = 1;
                    break;
                case GLUT_KEY_RIGHT:
					m_iYawRotation = -1;
                    break;
            }
            break;
        case Msg_SpecialKeyRelease: // Stop Move and Yaw
            switch (DereferenceToType<int>(message.ExtraInfo))
			{
                case GLUT_KEY_UP:
                case GLUT_KEY_DOWN:
					m_iThrust = 0;
                    break;
                case GLUT_KEY_LEFT:
                case GLUT_KEY_RIGHT:
					m_iYawRotation = 0;
                    break;
            }
        case Msg_CtrlMod_SpecialKeyRelease: // Stop all
            switch (DereferenceToType<int>(message.ExtraInfo))
			{
                case GLUT_KEY_UP:
                case GLUT_KEY_DOWN:
                    m_iPitchRotation = 0;
					m_iThrust = 0;
                    break;
                case GLUT_KEY_LEFT:
                case GLUT_KEY_RIGHT:
					m_iRollRotation = 0;
					m_iYawRotation = 0;
                    break;
            }
            break;
        case Msg_ShipSpeedChange:
            NextShipSpeed();
            break;
        case Msg_ShipWarp:
            WarpToCamera((DynamicCamera*)message.ExtraInfo);
            break;
        case Msg_ToggleGravity:
            m_bGravityStatus = !m_bGravityStatus;
            break;
        case Msg_ShipFireMissile:
            FireMissile();
            break;
        case Msg_DestroySource: // Destroys source
			printf("Msg: Destroy source received by ship ID: %d\n", m_iID);
			if (m_pActiveMissile && message.Sender == m_pActiveMissile->ID())
			{
				m_pActiveMissile = NULL;
				Scene* scene = Scene::Instance();
				if (m_iNumMissiles <= 0 &&
					((MissileBattery*)scene->GetEntityFromID(6) || (MissileBattery*)scene->GetEntityFromID(7)))
				{
                    FailMission();
				}
			}
			else
			{
				if (m_pTargets->size() > 0)
				{
					for (auto it = m_pTargets->begin(); it != m_pTargets->end(); it++)
					{
						if (message.Sender == (*it)->ID())
						{
							m_pTargets->erase(it);
							break;
						}
					}
				}
			}
            break;
        case Msg_TargetDestroyed: // Ship destroyed
			printf("Msg: Destroy target received by ship ID: %d\n", m_iID);
            printf("Ship hit by missile! Failing mission.\n");
            FailMission();
			break;

        default:
            hasMsg = false;
            break;

    }
    return hasMsg;
}

void Ship::NextShipSpeed()
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

void Ship::WarpToCamera(DynamicCamera* warpPoint)
{
    m_vPosition = warpPoint->Eye();
    m_vForward = glm::normalize(warpPoint->At() - warpPoint->Eye());
    m_vUp = warpPoint->Up();
    m_vLeft = glm::normalize(glm::cross(m_vUp, m_vForward));
    SetHeading();
    CreateRotationMatrix();
}

glm::vec3 Ship::GravityVector()
{
    float distance = glm::length(m_vPosition);
    glm::vec3 gravityVec = glm::normalize(m_vPosition * -1.0f) * (GRAVITY / (distance * distance));
    return gravityVec;
}

void Ship::FireMissile()
{
    if (!m_pActiveMissile)
    {
        if (m_iNumMissiles > 0)
        {
			printf("Missile launced by ship ID: %d\n", m_iID);
            m_pActiveMissile = new Missile(Scene::Instance()->GetModel("Missile"), m_vPosition, glm::vec3(25.0f),
                m_vPosition + m_vForward, m_vUp, 20.0f);
            m_pActiveMissile->SetTargets(m_pTargets);
            m_iNumMissiles--;
            printf("%d Missiles remaining!\n", m_iNumMissiles);
        }
		else
		{
            printf("No more missiles for ship to fire!\n");
        }
    }
	else
	{
        printf("Must wait for current missile to detonate!\n");
    }
}

void Ship::SetTargets(const std::string& type)
{
	for (auto entity : *Scene::Instance()->Entities())
	{
		if (StringICompare(type, entity.second->GetType()))
		{
			AddTarget((MoveableEntity*)entity.second);
		}
	}
}

void Ship::AddTarget(MoveableEntity* target)
{
    m_pTargets->push_back(target);
}

void Ship::RemoveTarget(MoveableEntity* target)
{
    for (auto it = m_pTargets->begin(); it != m_pTargets->end(); it++)
    {
        if (target == *it)
        {
            m_pTargets->erase(it);
            break;
        }
    }
}

void Ship::FailMission()
{
	printf("Msg: Destroy source sent by ship ID: %d\n", m_iID);
	MessageDispatcher::Instance()->DispatchMsg(0, m_iID, -1, Msg_DestroySource, NULL);
	m_pActiveMissile = NULL;
	m_pTargets->clear();
	Scene::Instance()->DestroyEntity(m_iID);
}

void Ship::CheckCollisions()
{
    for (int id : *Scene::Instance()->CollidableObjects())
	{
        // Skip self
		if (m_iID == id) continue;

        MoveableEntity* obj = (MoveableEntity*)Scene::Instance()->GetEntityFromID(id);
		std::string type = obj->GetType();

		if ("Missile" == type)
		{
			continue;
		}

        // Object hit?
		float distance = glm::distance(m_vPosition, obj->Position());
		if (distance <= m_fBoundingRadius + 10 + obj->BoundingRadius())
		{
			printf("Ship ID: %d, hits object of type %s\n", m_iID, type.c_str());

			// If destructable type, then destroy
			if ("MissileBattery" == type)
			{
				printf("Msg: Destroy target sent by ship ID: %d\n", m_iID);
				MessageDispatcher::Instance()->DispatchMsg(0, m_iID, obj->ID(), Msg_TargetDestroyed, NULL);
			}
            FailMission();
		}
	}
}
