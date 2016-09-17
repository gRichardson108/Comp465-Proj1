#include "Model.hpp"

Model::Model(char* modelFile, int vertices,
	GLuint* VAO, GLuint* buffer, GLuint* shaderProgram,
	GLuint* position, GLuint* color, GLuint* normal) :
	m_iVertices(vertices),
	m_gliVAO(VAO),
	m_gliBuffer(buffer),
	m_gliShaderProgram(shaderProgram),
	m_gliPosition(position),
	m_gliColor(color),
	m_gliNormal(normal),
	m_fBoundingRadius(0.0)
{
	size_t size = strlen(modelFile) + 1;
	m_cModelFile = new char[size];
	strcpy_s(m_cModelFile, size, modelFile);
}

Model::~Model()
{
	delete[] m_cModelFile;
}

void Model::Init()
{
	m_fBoundingRadius = loadModelBuffer(m_cModelFile, m_iVertices,
		*m_gliVAO, *m_gliBuffer, *m_gliShaderProgram, *m_gliPosition, *m_gliColor, *m_gliNormal,
		"vPosition", "vColor", "vNormal");
}
