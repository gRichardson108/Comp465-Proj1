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
        float movementRate = 0.0;
		Ship(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
			const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

        void keypress(unsigned char key, int x, int y);

        void rotateYaw(float degrees);

		virtual const std::string GetType() const { return "Ship"; }
		virtual bool HandleMsg(const Message& message) { return false; };

		void Update();
};

#endif
