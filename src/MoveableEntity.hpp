/*
Kory Dondzila
Garret Richardson

MoveableEntity.hpp
10/08/2016

Moveable entity class, used for displaying moveable objects in the scene.
This is meant to be an abstract class.

*/

#ifndef MOVEABLE_ENTITY_H
#define MOVEABLE_ENTITY_H

#include "BaseEntity.hpp"

class MoveableEntity : public BaseEntity
{
protected :
	glm::vec3 m_vHeading; // The direction Entity is moving

public :
    MoveableEntity(Model* model, const glm::vec3& pos = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f), 
		const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) :
		BaseEntity(model, pos, scale, target, up),
		m_vHeading(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		Scene::Instance()->AddMoveable(this);
	}

    virtual ~MoveableEntity(){}
	virtual void Update() = 0;
	virtual glm::vec3 Heading() { return m_vHeading; }
	virtual void SetHeading(const glm::vec3& heading) { m_vHeading = heading; }
};

#endif
