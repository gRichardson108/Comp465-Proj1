/*
Kory Dondzila
Garret Richardson

Missile.hpp
10/08/2016

Handles any missile object in the scene, will know what
object spawned it and have a lifetime. Can be affected by
gravity.

*/

#ifndef MISSILE_H
#define MISSILE_H

#include "MoveableEntity.hpp"

class Missile : public MoveableEntity
{
public:
	Missile(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
		const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	void Update() {}
};

#endif
