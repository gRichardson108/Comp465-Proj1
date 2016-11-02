#include "Scene.hpp"
#include "Model.hpp"
#include "MoveableEntity.hpp"
#include "DynamicCamera.hpp"

void Scene::Update()
{
	UnloadQueues();

	// Update entities
	m_itMoveableEntity = m_pMoveableEntities->begin();
	do
	{
		(*m_itMoveableEntity)->Update();
	} while (++m_itMoveableEntity != m_pMoveableEntities->end());

	// Update cameras
	m_itDynamicCamera = m_pDynamicCameras->begin();
	do
	{
		(*m_itDynamicCamera)->Update();
	} while (++m_itDynamicCamera != m_pDynamicCameras->end());
}

void Scene::UnloadQueues()
{
	while (!m_pMoveableEntitesQueue->empty())
	{
		m_pMoveableEntities->push_back(m_pMoveableEntitesQueue->front());
		m_pMoveableEntitesQueue->pop();
	}
	while (!m_pDynamicCamerasQueue->empty())
	{
		m_pDynamicCameras->push_back(m_pDynamicCamerasQueue->front());
		m_pDynamicCamerasQueue->pop();
	}
}

Model* Scene::GetModel(char* name)
{
	for (std::vector<Model*>::iterator it = m_pModels->begin(); it != m_pModels->end(); it++)
	{
		std::string file = (*it)->ModelFile();
		size_t f1 = file.find_last_of('/');
		size_t f2 = file.find_last_of('.');
		file = file.substr(f1 + 1, f2 - f1 - 1);
		const char* modelName = file.c_str();
		if (_stricmp(modelName, name) == 0)
		{
			return (*it);
		}
	}

	return NULL;
}
