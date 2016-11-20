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
    int pitchRotation = 0;
    int yawRotation = 0;
    int rollRotation = 0;
    int thrust = 0;
    int m_iNumMissiles = 9; // Number of remaining missiles
	std::vector<MoveableEntity*>* m_pTargets  = new std::vector<MoveableEntity*>(); // Vector of possible targets
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

    const float TURN_RATE = 0.02;//radians
    const float GRAVITY = 90000000.0;
    bool gravityStatus = false;

    Ship(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
         const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    void keypress(unsigned char key);

    void rotateYaw(float rotationRate);
    void rotatePitch(float rotationRate);
    void rotateRoll(float rotationRate);

    void nextShipSpeed();
    void warpToCamera(DynamicCamera* warpPoint);

    virtual const std::string GetType() const
    {
        return "Ship";
    }
    virtual bool HandleMsg(const Message& message);

    void SetHeading();
    glm::vec3 gravityVector();

    void Update();

    void fireMissile();

    void AddTarget(MoveableEntity* target);

	void RemoveTarget(MoveableEntity* target);

	void FailMission();

	int NumMissiles();

	//Check collisions with celestial bodies
	void CheckCollisions();

};

#endif
