#ifndef SCENE_H
#define SCENE_H

#include "MoveableEntity.hpp"

class Scene
{
private:
	int m_iTimerDelay; // In miliseconds
	static Scene* s_pInstance; // Singleton
	BaseEntity** m_pEntities; // Pointer array of scene entities
	MoveableEntity** m_pMoveableEntities; // Pointer array of moveable scene entities
	int m_iNumEntities; // Number of entities
	int m_iNumMoveableEntities;// Number of moveable entities

public:
	Scene(int delay = 5) :
		m_iTimerDelay(delay)
	{
		s_pInstance = this;
		m_pEntities = NULL;
		m_pMoveableEntities = NULL;
		m_iNumEntities = 0;
		m_iNumMoveableEntities = 0;
	}

	~Scene()
	{
		delete* m_pEntities;
		delete* m_pMoveableEntities;
	}

	static Scene* Instance()
	{
		if (!s_pInstance)
			s_pInstance = new Scene;
		return s_pInstance;
	}

	int TimerDelay() { return m_iTimerDelay; }
	void SetTimerDelay(int delay)
	{
		m_iTimerDelay = delay;
		for (int i = 0; i < m_iNumMoveableEntities; i++)
		{
			m_pMoveableEntities[i]->UpdateTimeDelay();
		}
	}
	
	BaseEntity** Entities() { return m_pEntities; }
	void SetEntities(BaseEntity* entities[], int num)
	{
		m_iNumEntities = num;
		m_pEntities = entities;
	}

	MoveableEntity** MoveableEntities() { return m_pMoveableEntities; }
	void SetMoveables(MoveableEntity* entities[], int num)
	{
		m_iNumMoveableEntities = num;
		m_pMoveableEntities = entities;
	}
};

#endif
