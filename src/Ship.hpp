/*
Kory Dondzila
Garret Richardson

Ship.hpp
10/08/2016

Ship class that handles any input for movement and
can have missiles to shoot. Can be affected by gravity.

*/

#ifndef SHIP_H
#define SHIP_H

#include "MoveableEntity.hpp"
#include "Missile.hpp"

class Ship : public MoveableEntity
{
private:
    signed char m_iPitchRotation;
	signed char m_iYawRotation;
	signed char m_iRollRotation;
	signed char m_iThrust;
    int m_iNumMissiles; // Number of remaining missiles
	std::vector<MoveableEntity*>* m_pTargets; // Vector of possible targets
	Missile* m_pActiveMissile;

public:
    enum shipSpeedSettings
    {
        SPEED_DEFAULT,
        SPEED_FAST,
        SPEED_DEBUG
    };

    shipSpeedSettings speedSetting = SPEED_DEFAULT;
    float currentMaxSpeed = 10.0;

    const float TURN_RATE = 0.02f;//radians
    const float GRAVITY = 90000000.0f;
    bool m_bGravityStatus = false;

    Ship(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
         const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	~Ship()
	{
		delete m_pTargets;
	}

	virtual const std::string GetType() const { return "Ship"; }
	virtual bool HandleMsg(const Message& message);

	void Update();

    void RotateYaw(float rotationRate);
    void RotatePitch(float rotationRate);
    void RotateRoll(float rotationRate);

    void NextShipSpeed();
    void WarpToCamera(DynamicCamera* warpPoint);

    void SetHeading();
    glm::vec3 GravityVector();

    void FireMissile();
	void SetTargets(const std::string& type);
    void AddTarget(MoveableEntity* target);
	void RemoveTarget(MoveableEntity* target);

	void FailMission();

	int NumMissiles() const { return m_iNumMissiles; }

	//Check collisions with celestial bodies
	void CheckCollisions();
};

#endif
