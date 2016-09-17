#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "Model.hpp"
#include <glm\glm.hpp>

class BaseEntity
{
protected:
	glm::vec3 m_vPosition;
	glm::vec3 m_vForward;
	glm::vec3 m_vLeft;
	glm::vec3 m_vUp;
	glm::vec3 m_vScale;
	float m_fBoundingRadius;
	Model* m_mModel;

public:
	BaseEntity(Model* model) :
		m_vPosition(glm::vec3()),
		m_vScale(glm::vec3(1.0, 1.0, 1.0)),
		m_vForward(glm::vec3(0.0, 0.0, -1.0)),
		m_vLeft(glm::vec3(-1.0, 0.0, 0.0)),
		m_vUp(glm::vec3(0.0, 1.0, 0.0)),
		m_mModel(model)
	{}

	BaseEntity(Model* model, glm::vec3 pos) :
		m_vPosition(pos),
		m_vScale(glm::vec3(1.0, 1.0, 1.0)),
		m_vForward(glm::vec3(0.0, 0.0, -1.0)),
		m_vLeft(glm::vec3(-1.0, 0.0, 0.0)),
		m_vUp(glm::vec3(0.0, 1.0, 0.0)),
		m_mModel(model)
	{}

	BaseEntity(Model* model, glm::vec3 pos, glm::vec3 scale) :
		m_vPosition(pos),
		m_vForward(glm::vec3(0.0, 0.0, -1.0)),
		m_vLeft(glm::vec3(-1.0, 0.0, 0.0)),
		m_vUp(glm::vec3(0.0, 1.0, 0.0)),
		m_mModel(model)
	{
		SetScale(scale);
	}

	BaseEntity(Model* model, glm::vec3 pos, glm::vec3 scale, glm::vec3 target, glm::vec3 worldUp) :
		m_vPosition(pos),
		m_mModel(model)
	{
		SetScale(scale);
		m_vForward = glm::normalize(target - pos);
		m_vLeft = glm::cross(glm::normalize(worldUp), m_vForward);
		m_vUp = glm::cross(m_vForward, m_vLeft);
	}

	virtual ~BaseEntity() {}

	glm::vec3 Position() const { return m_vPosition; }
	void SetPosition(glm::vec3 position) { m_vPosition = position; }
	void SetPosition(float x, float y, float z) { m_vPosition = glm::vec3(x, y, z); }

	glm::vec3 Scale() const { return m_vScale; }
	void SetScale(float scale) { SetScale(glm::vec3(scale)); }
	void SetScale(float x, float y, float z) { SetScale(glm::vec3(x, y, z)); }
	void SetScale(glm::vec3 scale)
	{
		m_vScale = scale * 1.0f / m_mModel->BoundingRadius();
		m_fBoundingRadius = glm::max(m_vScale.x, glm::max(m_vScale.y, m_vScale.z));
	}

	glm::vec3 Forward() { return m_vForward; }
	glm::vec3 Backward() { return -m_vForward; }
	glm::vec3 Left() { return m_vLeft; }
	glm::vec3 Right() { return -m_vLeft; }
	glm::vec3 Up() { return m_vUp; }
	glm::vec3 Down() { return -m_vUp; }

	float BoundingRadius() { return m_fBoundingRadius; }
	Model* ModelFile() { return m_mModel; }
};

#endif
