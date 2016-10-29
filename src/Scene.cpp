#include "Scene.hpp"
#include "Model.hpp"

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
