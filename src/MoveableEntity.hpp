#ifndef MOVEABLE_ENTITY_H
#define MOVEABLE_ENTITY_H

#include "BaseEntity.hpp"

class MoveableEntity : public BaseEntity
{
protected :
	glm::vec3 m_vHeading; // The direction Entity is moving

public : 

    MoveableEntity(Model* model) : BaseEntity(model),
		m_vHeading(glm::vec3(0.0f, 0.0f, -1.0f))
	{}

    MoveableEntity(Model* model, const glm::vec3& pos) : BaseEntity(model, pos),
		m_vHeading(glm::vec3(0.0f, 0.0f, -1.0f))
	{}

    MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale) 
		: BaseEntity(model, pos, scale),
		m_vHeading(glm::vec3(0.0f, 0.0f, -1.0f))
	{}

    MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) 
		: BaseEntity(model, pos, scale, target, up),
		m_vHeading(glm::vec3(0.0f, 0.0f, -1.0f))
	{}

    virtual ~MoveableEntity(){}
	virtual void Update() = 0;
	virtual glm::vec3 Heading() { return m_vHeading; }
	virtual void SetHeading(const glm::vec3& heading) { m_vHeading = heading; }
};

#endif
