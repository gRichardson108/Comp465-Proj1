#include "MoveableEntity.hpp"

glm::mat4 MoveableEntity::ModelMatrix()
{

    return glm::translate(glm::mat4(), m_vPosition) * rotationMatrix * glm::scale(glm::mat4(),m_vScale);
}
