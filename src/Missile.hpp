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
private:
	const int m_iTotalLifeTime = 2000; // Total amount of life each missile has
	int m_iLifeTime; // Number of updates the missile has to live
	bool m_bLive; // Is missile live?
	MoveableEntity* m_pCurrentTarget; // Current tracked target
	MoveableEntity** m_pTargets; // Targets that can be tracked (passed in from what is firing)
	int m_iNumTargets; // Number of  targets
	float m_fVelocity; // Velocity of the missile

public:
	Missile(Model* model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f), 
		const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	~Missile()
	{
		delete* m_pTargets;
	}

	void Update() {}
	MoveableEntity* CurrentTarget() { return m_pCurrentTarget; }
	MoveableEntity** Targets() { return m_pTargets; }
	void SetTargets(MoveableEntity** targets, int numTargets)
	{
		m_pTargets = targets;
		m_pCurrentTarget = NULL;
	}
};

#endif
