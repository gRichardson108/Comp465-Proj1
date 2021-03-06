#include "Scene.hpp"
#include "Model.hpp"
#include "BaseEntity.hpp"
#include "MoveableEntity.hpp"
#include "DynamicCamera.hpp"

Scene* Scene::s_pInstance = NULL;

Scene::~Scene()
{
	// Clear and delete sets
    m_pStaticEntities->clear();
    delete m_pStaticEntities;
    m_pMoveableEntities->clear();
    delete m_pMoveableEntities;
    m_pStaticCameras->clear();
    delete m_pStaticCameras;
    m_pDynamicCameras->clear();
    delete m_pDynamicCameras;
    m_pDestroyedEntities->clear();
    delete m_pDestroyedEntities;

	// Remove anything from queues and delete
    while (!m_pDynamicCamerasQueue->empty())
    {
        delete m_pDynamicCamerasQueue->front();
        m_pDynamicCamerasQueue->pop();
    }
    delete m_pDynamicCamerasQueue;

    while (!m_pMoveableEntitesQueue->empty())
    {
        delete m_pMoveableEntitesQueue->front();
        m_pMoveableEntitesQueue->pop();
    }
    delete m_pMoveableEntitesQueue;

	// Delete all entities and map
    for (auto i : *m_pEntities)
    {
        delete i.second;
    }
    m_pEntities->clear();
    delete m_pEntities;

	// Delete all models
    for (auto i : *m_pModels)
    {
        delete i.second;
    }
    m_pModels->clear();
    delete m_pModels;
}

void Scene::AddEntity(BaseEntity* entity)
{
    m_pEntities->insert(std::make_pair(entity->ID(), entity));
}

void Scene::AddToMoveableQueue(MoveableEntity* entity)
{
	// Add to queue if not in init otherwise
	// add directly to set and map
    if (!m_bInit)
    {
        m_pMoveableEntitesQueue->push(entity);
    }
    else
    {
        m_pMoveableEntities->insert(entity->ID());
        m_pEntities->insert(std::make_pair(entity->ID(), entity));
    }
}

void Scene::AddToDynamicQueue(DynamicCamera* entity)
{
	// Add to queue if not in init otherwise
	// add directly to set and map
    if (!m_bInit)
    {
        m_pDynamicCamerasQueue->push(entity);
    }
    else
    {
        m_pDynamicCameras->insert(entity->ID());
        m_pEntities->insert(std::make_pair(entity->ID(), entity));
        if (entity->m_bWarpPoint)
        {
            m_pWarpPoints->insert(entity->ID());
        }
    }
}

void Scene::Update()
{
    Preprocess();

    // Update moveable entities
    for (int id : *m_pMoveableEntities)
    {
        if (m_pDestroyedEntities->find(id) == m_pDestroyedEntities->end())
        {
            ((MoveableEntity*)m_pEntities->at(id))->Update();
        }
    }

    // Update dynamic cameras
    for (int id : *m_pDynamicCameras)
    {
        if (m_pDestroyedEntities->find(id) == m_pDestroyedEntities->end())
        {
            ((DynamicCamera*)m_pEntities->at(id))->Update();
        }
    }
}

void Scene::Preprocess()
{
    // Remove all destroyed entities
    if (m_pDestroyedEntities->size() > 0)
    {
        for (int id : *m_pDestroyedEntities)
        {
            m_pStaticEntities->erase(id);
            m_pMoveableEntities->erase(id);
            m_pDynamicCameras->erase(id);

            // Change camera if necessary
            if (*m_itViewingCamera == id)
            {
                NextCamera();
            }

            m_pStaticCameras->erase(id);
            delete m_pEntities->at(id);
            m_pEntities->erase(id);
        }

        m_pDestroyedEntities->clear();
    }

	// Empty queues into sets and map
    while (!m_pMoveableEntitesQueue->empty())
    {
        MoveableEntity* entity = m_pMoveableEntitesQueue->front();
        m_pMoveableEntities->insert(entity->ID());
        m_pEntities->insert(std::make_pair(entity->ID(), entity));
        m_pMoveableEntitesQueue->pop();
    }

    while (!m_pDynamicCamerasQueue->empty())
    {
        DynamicCamera* entity = m_pDynamicCamerasQueue->front();
        m_pDynamicCameras->insert(entity->ID());
        m_pEntities->insert(std::make_pair(entity->ID(), entity));
        m_pDynamicCamerasQueue->pop();
    }
}

void Scene::AddModel(Model* model)
{
    std::string file = model->ModelFile();
    size_t f1 = file.find_last_of('/');
    size_t f2 = file.find_last_of('.');
    file = file.substr(f1 + 1, f2 - f1 - 1);
    m_pModels->insert(std::make_pair(file, model));
}
