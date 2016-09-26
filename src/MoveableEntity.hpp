#ifndef MOVEABLE_ENTITY_H
#define MOVEABLE_ENTITY_H

#include "BaseEntity.hpp"

class MoveableEntity : public BaseEntity
{
    public : 

        MoveableEntity(Model* model) : BaseEntity(model)
		{}

        MoveableEntity(Model* model, const glm::vec3& pos) : BaseEntity(model, pos)
		{}

        MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale) 
			: BaseEntity(model, pos, scale)
		{}

        MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) 
			: BaseEntity(model, pos, scale, target, up)
		{}

        virtual ~MoveableEntity(){}

		virtual void Update() = 0;
};

#endif
