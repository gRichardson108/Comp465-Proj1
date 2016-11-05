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

	virtual const char* GetType() { return "MissileBattery"; }
	virtual bool HandleMsg(const Message& message);

	void Update();

	int NumMissiles() const { return m_iNumMissiles; }
	void SetTargets(std::vector<MoveableEntity*>* targets)
	{
		m_pTargets = targets;
	}
	void SetTargets(const char* type)
	{
		for each (auto entity in *Scene::Instance()->Entities())
		{
			// Can't use stricmp on VS what's best option?
			if (_stricmp(type, entity.second->GetType()) == 0)
			{
				AddTarget((MoveableEntity*)entity.second);
			}
		}
	}
	void AddTarget(MoveableEntity* target)
	{
		m_pTargets->push_back(target);
	}
	void RemoveTarget(MoveableEntity* target)
	{
		for (std::vector<MoveableEntity*>::iterator it = m_pTargets->begin(); it != m_pTargets->end(); it++)
		{
			if (target == *it)
			{
				m_pTargets->erase(it);
				break;
			}
		}
	}
};

#endif
