#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "includes465\glmUtils465.hpp"
#include "Model.hpp"

class BaseEntity
{
protected:
	Model * m_mModel;
	glm::vec3 m_vPosition;
	glm::vec3 m_vScale;
	float m_fBoundingRadius;

public:
	BaseEntity() :m_vPosition(glm::vec3()),
		m_vScale(glm::vec3(1.0, 1.0, 1.0))
	{}

	BaseEntity(glm::vec3 pos) :m_vPosition(pos),
		m_vScale(glm::vec3(1.0, 1.0, 1.0))
	{}

	BaseEntity(glm::vec3 pos, glm::vec3 scale) :m_vPosition(pos),
		m_vScale(scale)
	{}

	virtual ~BaseEntity() {}

	glm::vec3 Position() const { return m_vPosition; }
	void SetPosition(glm::vec3 position) { m_vPosition = position; }
	void SetPosition(float x, float y, float z) { m_vPosition = glm::vec3(x, y, z); }

	glm::vec3 Scale() const { return m_vScale; }
	void SetScale(float scale) { SetScale(glm::vec3(scale)); }
	void SetScale(float x, float y, float z) { SetScale(glm::vec3(x, y, z)); }
	void SetScale(glm::vec3 scale) { m_vScale = scale * 1.0f / m_mModel->BoundingRadius;
		m_fBoundingRadius = glm::max(m_vScale.x, glm::max(m_vScale.y, m_vScale.z));
	}
};

#endif
