#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include "MoveableEntity.h"
#include <math.h> //for cosine and sine

class CelestialBody : public MoveableEntity
{
    protected :
        float selfRotationRate;//rate we rotate on our axis
        float orbitRate;//speed we orbit our parent, if any
        CelestialBody bodyOrbiting;//if we're orbiting another body, which one?


}



#endif
