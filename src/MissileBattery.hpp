/*
Kory Dondzila
Garret Richardson

MissileBattery.hpp
11/18/2016

Missile battery attaches to an object and shoots missiles

*/

#ifndef MISSILE_BATTERY_H
#define MISSILE_BATTERY_H

#include "MoveableEntity.hpp"
#include "Missile.hpp"
#include "CelestialBody.hpp"

class MissileBattery : public MoveableEntity
{
private:
	int m_iNumMissiles; // Number of remaining missiles
	CelestialBody* m_pParent; // Entity missile battery is attached to
	glm::vec3 m_vParentOldPosition; // Parent's old position
	std::vector<MoveableEntity*>* m_pTargets; // Vector of possible targets
	Missile* m_pActiveMissile;

	void FireMissile(); // Launches a missile

public:
	MissileBattery(Model* model, CelestialBody* parent = NULL, const glm::vec3& pos = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f),
		const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	~MissileBattery()
	{
		delete m_pTargets;
	}

	virtual const std::string GetType() const { return "MissileBattery"; }
	virtual bool HandleMsg(const Message& message);

	void Update();

	int NumMissiles() const { return m_iNumMissiles; }
	void SetTargets(const std::string& type);
	void AddTarget(MoveableEntity* target);
	void RemoveTarget(MoveableEntity* target);
};

#endif
