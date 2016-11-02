/*
Kory Dondzila
Garret Richardson

Scene.hpp
10/08/2016

Scene singleton, stores information about what is in the scene,
update delay, entities.

*/

#ifndef SCENE_H
#define SCENE_H

#include "../includes465/include465.hpp"

class Model;
class BaseEntity;
class MoveableEntity;
class StaticCamera;
class DynamicCamera;

class Scene
{
private:
	int m_iTimerDelay; // Delay in miliseconds
	bool m_bInit;
	static Scene* s_pInstance; // Singleton
	std::vector<Model*>* m_pModels; // Pointer to vector of models
	std::vector<BaseEntity*>* m_pEntities; // Pointer to vector of scene entities
	std::vector<MoveableEntity*>* m_pMoveableEntities; // Pointer to vector of moveable scene entities
	std::vector<MoveableEntity*>::iterator m_itMoveableEntity; // Moveable entities iterator
	std::queue<MoveableEntity*>* m_pMoveableEntitesQueue; // Moveables to add to vector
	std::vector<StaticCamera*>* m_pStaticCameras; // Pointer to vector of static cameras
	std::vector<DynamicCamera*>* m_pDynamicCameras; // Pointer to vector of dynamic cameras
	std::vector<DynamicCamera*>::iterator m_itDynamicCamera; // Dynamic cameras iterator
	std::queue<DynamicCamera*>* m_pDynamicCamerasQueue; // Cameras to add to vector
	std::vector<StaticCamera*>::iterator m_itViewingCamera; // Current camera iterator

	void UnloadQueues();

public:
	Scene(int delay = 5) :
		m_iTimerDelay(delay),
		m_bInit(true)
	{
		s_pInstance = this;
		m_pModels = new std::vector<Model*>();
		m_pEntities = new std::vector<BaseEntity*>();
		m_pMoveableEntities = new std::vector<MoveableEntity*>();
		m_pMoveableEntitesQueue = new std::queue<MoveableEntity*>();
		m_pStaticCameras = new std::vector<StaticCamera*>();
		m_pDynamicCameras = new std::vector<DynamicCamera*>();
		m_pDynamicCamerasQueue = new std::queue<DynamicCamera*>();
	}

	~Scene()
	{
		delete m_pModels;
		delete m_pEntities;
		delete m_pMoveableEntities;
		delete m_pMoveableEntitesQueue;
		delete m_pStaticCameras;
		delete m_pDynamicCameras;
		delete m_pDynamicCamerasQueue;
		delete s_pInstance;
	}

	static Scene* Instance()
	{
		if (!s_pInstance)
			s_pInstance = new Scene;
		return s_pInstance;
	}

	void Update();

	int TimerDelay() { return m_iTimerDelay; }
	void SetTimerDelay(int delay) { m_iTimerDelay = delay; }
	void InitDone() { m_bInit = false; }

	std::vector<Model*>* Models() { return m_pModels; }
	void AddModel(Model* model)
	{
		m_pModels->push_back(model);
	}
	void RemoveEntity(Model* model)
	{
		for (std::vector<Model*>::iterator it = m_pModels->begin(); it != m_pModels->end(); it++)
		{
			if (model == *it)
			{
				m_pModels->erase(it);
				break;
			}
		}
	}
	Model* GetModel(char* name);
	
	std::vector<BaseEntity*>* Entities() { return m_pEntities; }
	void AddEntity(BaseEntity* entity)
	{
		m_pEntities->push_back(entity);
	}
	void RemoveEntity(BaseEntity* entity)
	{
		for (std::vector<BaseEntity*>::iterator it = m_pEntities->begin(); it != m_pEntities->end(); it++)
		{
			if (entity == *it)
			{
				m_pEntities->erase(it);
				break;
			}
		}
	}
	BaseEntity* GetEntity(int i) { return m_pEntities->at(i); }

	std::vector<MoveableEntity*>* MoveableEntities() { return m_pMoveableEntities; }
	void AddMoveable(MoveableEntity* entity)
	{
		m_pMoveableEntities->push_back(entity);
		m_itMoveableEntity = m_pMoveableEntities->begin();
	}
	void RemoveEntity(MoveableEntity* entity)
	{
		for (std::vector<MoveableEntity*>::iterator it = m_pMoveableEntities->begin(); it != m_pMoveableEntities->end(); it++)
		{
			if (entity == *it)
			{
				m_pMoveableEntities->erase(it);
				break;
			}
		}
	}
	MoveableEntity* GetMoveableEntity(int i) { return m_pMoveableEntities->at(i); }

	void AddToMoveableQueue(MoveableEntity* entity)
	{
		if (!m_bInit)
		{
			m_pMoveableEntitesQueue->push(entity);
		}
		else
		{
			m_pMoveableEntities->push_back(entity);
		}
	}

	std::vector<StaticCamera*>* StaticCameras() { return m_pStaticCameras; }
	void AddStaticCamera(StaticCamera* entity)
	{
		m_pStaticCameras->push_back(entity);
		m_itViewingCamera = m_pStaticCameras->begin();
	}
	void RemoveStaticCamera(StaticCamera* entity)
	{
		for (std::vector<StaticCamera*>::iterator it = m_pStaticCameras->begin(); it != m_pStaticCameras->end(); it++)
		{
			if (entity == *it)
			{
				m_pStaticCameras->erase(it);
				break;
			}
		}
	}
	StaticCamera* ViewingCamera() { return *m_itViewingCamera; }
	StaticCamera* NextCamera()
	{
		m_itViewingCamera++;
		if (m_itViewingCamera == m_pStaticCameras->end())
		{
			m_itViewingCamera = m_pStaticCameras->begin();
		}

		return *m_itViewingCamera;
	}
	StaticCamera* PrevCamera()
	{
		if (m_itViewingCamera == m_pStaticCameras->begin())
		{
			m_itViewingCamera = m_pStaticCameras->end();
		}

		m_itViewingCamera--;
		return *m_itViewingCamera;
	}

	std::vector<DynamicCamera*>* DynamicCameras() { return m_pDynamicCameras; }
	void AddDynamicCamera(DynamicCamera* entity)
	{
		m_pDynamicCameras->push_back(entity);
		m_itDynamicCamera = m_pDynamicCameras->begin();
	}
	void RemoveDynamicCamera(DynamicCamera* entity)
	{
		for (std::vector<DynamicCamera*>::iterator it = m_pDynamicCameras->begin(); it != m_pDynamicCameras->end(); it++)
		{
			if (entity == *it)
			{
				m_pDynamicCameras->erase(it);
				break;
			}
		}
	}

	void AddToDynamicQueue(DynamicCamera* entity)
	{
		if (!m_bInit)
		{
			m_pDynamicCamerasQueue->push(entity);
		}
		else
		{
			m_pDynamicCameras->push_back(entity);
		}
	}
};

#endif
