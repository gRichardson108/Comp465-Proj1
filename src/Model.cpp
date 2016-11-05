#include "Model.hpp"
#include "Scene.hpp"

Model::Model(char* modelFile, int vertices,
	GLuint* VAO, GLuint* buffer, GLuint* shaderProgram) :
	m_iVertices(vertices),
	m_gliVAO(VAO),
	m_gliBuffer(buffer),
	m_gliShaderProgram(shaderProgram),
	m_fBoundingRadius(0.0)
{
	size_t size = strlen(modelFile) + 1;
	m_cModelFile = new char[size];
	strncpy(m_cModelFile, modelFile, size);
	Init();
	Scene::Instance()->AddModel(this);
}

void Model::Init()
{
	m_fBoundingRadius = loadModelBuffer(m_cModelFile, m_iVertices,
		*m_gliVAO, *m_gliBuffer, *m_gliShaderProgram, GLuint(), GLuint(), GLuint(),
		"vPosition", "vColor", "vNormal");
}
