#ifndef MOVEABLE_ENTITY_H
#define MOVEABLE_ENTITY_H

#include "BaseEntity.hpp"

class MoveableEntity : public BaseEntity
{
    protected : 
        glm::mat4 rotationMatrix;


    public : 

        //We default the model to face its forward vector
        MoveableEntity(Model* model) : BaseEntity(model)
    {
        rotationMatrix = RotateToForward();
    }

        MoveableEntity(Model* model, const glm::vec3& pos) : BaseEntity(model, pos)
    {
        rotationMatrix = RotateToForward();
    }

        MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale) : BaseEntity(model, pos, scale){
            rotationMatrix = RotateToForward();
        }

        MoveableEntity(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) : BaseEntity(model, pos, scale, target, up) {
            rotationMatrix = RotateToForward();
        }

        virtual ~MoveableEntity();

        virtual void update() =0;

        glm::mat4 ModelMatrix();
};

#endif
