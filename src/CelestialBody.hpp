#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include "MoveableEntity.hpp"

class CelestialBody : public MoveableEntity
{
    private :
		CelestialBody* m_eParent;//if we're orbiting another body, which one?
		glm::vec3 m_vParentOldPosition;
        float m_fRotationRate;//rate we rotate on our axis
		glm::mat3 m_mRotation;
		float m_fOrbitDistance;
        float m_fOrbitRate;//speed we orbit our parent, if any
        glm::vec3 m_vOrbitAxis;//the axis we orbit our parent, if any
		glm::mat3 m_mOrbit;
        
    public :
        //bunch of boilerplate constructors that pass parameters up the hierarchy
		CelestialBody(Model* model, CelestialBody* parent = NULL, const glm::vec3& pos = glm::vec3(0.0f),
			const glm::vec3& scale = glm::vec3(1.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f),
			const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float rotationRate = 0.0f, float orbitRate = 0.0f,
			const glm::vec3& orbitAxis = glm::vec3(0.0f, 1.0f, 0.0f));

        virtual ~CelestialBody() {}
        void Update();
		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }
};

#endif
