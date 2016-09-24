#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include "MoveableEntity.hpp"
#include <math.h> //for cosine and sine

class CelestialBody : public MoveableEntity
{
    protected :
        float selfRotationRate;//rate we rotate on our axis
        CelestialBody* bodyOrbiting;//if we're orbiting another body, which one?
        float orbitRate;//speed we orbit our parent, if any
        glm::vec3 rotationAxis;//the axis we rotate around ourselves
        glm::vec3 orbitAxis;//the axis we orbit our parent, if any

        void setDefaultRotation();

        glm::mat4 rotate();
        
    public :
        //bunch of boilerplate constructors that pass parameters up the hierarchy
        CelestialBody(Model* model) : MoveableEntity(model){setDefaultRotation();}
        CelestialBody(Model* model, const glm::vec3& pos) : MoveableEntity(model, pos){setDefaultRotation();}
        CelestialBody(Model* model, const glm::vec3& pos, const glm::vec3 scale) : MoveableEntity(model, pos, scale){setDefaultRotation();}
        CelestialBody(Model* model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& target, const glm::vec3& up) : MoveableEntity(model, pos, scale, target, up){setDefaultRotation();}

        void update();
};



#endif
