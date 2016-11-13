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

class Ship : public MoveableEntity
{
    private:
	public:
        enum shipSpeedSettings {
            SPEED_DEFAULT,
            SPEED_FAST,
            SPEED_DEBUG
        };

        float movementRate = 0.0;
        shipSpeedSettings speedSetting = SPEED_DEFAULT;
        float currentMaxSpeed = 10.0;

        const float TURN_RATE = 0.02;//radians
		Ship(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
			const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

        void keypress(unsigned char key);

        void rotateYaw(float rotationRate);
        void rotatePitch(float rotationRate);
        void rotateRoll(float rotationRate);

        void nextShipSpeed(){
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
        }

		virtual const std::string GetType() const { return "Ship"; }
		virtual bool HandleMsg(const Message& message);

		void Update();
};

#endif
