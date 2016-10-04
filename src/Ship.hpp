#ifndef SHIP_H
#define SHIP_H

#include "MoveableEntity.hpp"

class Ship : public MoveableEntity
{
private:

public:
	Ship(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
		const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	void Update() {}
};

#endif
